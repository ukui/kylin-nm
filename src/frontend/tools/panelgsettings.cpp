#include <glib.h>
#include <gio/gio.h>
#include "gobject/gobject.h"
#include <mutex>
#include <QDebug>

#include "panelgsettings.h"

static std::once_flag onceFlag;
static PanelGSettings* g_instance = nullptr;
static GSettings * m_settings = nullptr;
static GSettingsSchema * m_schema = nullptr;
static const char *m_panelLengthKey = "panellength";
PanelGSettings *PanelGSettings::instance()
{
    std::call_once(onceFlag, [ & ]
    {
        g_instance = new PanelGSettings();
    });
    return g_instance;
}

int PanelGSettings::getPanelLength(QString screenName)
{
    if (!m_settings || !m_schema) return -1;
    if (!isKeysContain(m_panelLengthKey)) return -1;

    QMap<QString, QVariant> map = getPanelLengthMap();
    if (!map.contains(screenName))
    {
        return -1;
    }
    return map.value(screenName).toInt();
}

PanelGSettings::~PanelGSettings()
{
    if (m_settings)
    {
        g_object_unref(m_settings);
    }
    if (m_schema)
    {
        g_settings_schema_unref(m_schema);
    }
    g_instance = nullptr;
}

PanelGSettings::PanelGSettings(QObject *parent) : QObject(parent)
{
    GSettingsSchemaSource *source;

    source = g_settings_schema_source_get_default();
    m_schema = g_settings_schema_source_lookup(source, "org.ukui.panel.settings", true);
    g_settings_schema_source_unref(source);

    if (!m_schema)
    {
        m_settings = nullptr;
        return;
    }

    m_settings = g_settings_new_with_path("org.ukui.panel.settings", "/org/ukui/panel/settings/");
}

bool PanelGSettings::isKeysContain(const char *key)
{
    if (!m_settings || !m_schema) return false;

    gchar **keys = g_settings_schema_list_keys(m_schema);
    if (g_strv_contains(keys, key))
    {
        g_strfreev(keys);
        return true;
    }
    else
    {
        g_strfreev(keys);
        return false;
    }
}

QMap<QString, QVariant> PanelGSettings::getPanelLengthMap()
{
    GVariant *gvalue = g_settings_get_value(m_settings, m_panelLengthKey);

    GVariantIter iter;
    QMap<QString, QVariant> map;
    const gchar *key;
    size_t str_len;
    GVariant *val = NULL;
    g_variant_iter_init (&iter, gvalue);
    QVariant qvar;

    while (g_variant_iter_next (&iter, "{&sv}", &key, &val))
    {
        if (g_variant_is_of_type(val, G_VARIANT_TYPE_UINT32))
        {
            qvar = QVariant::fromValue(static_cast<quint32>(g_variant_get_uint32(val)));
            map.insert(key, qvar);
        }
    }

    g_variant_unref(gvalue);

    return map;
}

