#include "test_listitemedit.h"

TestListItemEdit::TestListItemEdit(QObject *parent)
    : QObject(parent)
    , m_listItemEdit(nullptr)
    , m_model(nullptr)
{
}

void TestListItemEdit::initTestCase()
{
    // Initialize resources if needed
}

void TestListItemEdit::cleanupTestCase()
{
    // Cleanup resources if needed
}

void TestListItemEdit::init()
{
    QRegExp rx(".*");
    m_listItemEdit = new ListItemEdit(rx);
    m_model = new QStandardItemModel(1, 1);
    m_model->setData(m_model->index(0, 0), "test", Qt::EditRole);
}

void TestListItemEdit::cleanup()
{
    if (m_listItemEdit) {
        delete m_listItemEdit;
        m_listItemEdit = nullptr;
    }
    if (m_model) {
        delete m_model;
        m_model = nullptr;
    }
}

void TestListItemEdit::test_constructor()
{
    QRegExp rx(".*");
    ListItemEdit *editor = new ListItemEdit(rx);
    QVERIFY(editor != nullptr);
    delete editor;
}

void TestListItemEdit::test_destructor()
{
    QRegExp rx(".*");
    ListItemEdit *editor = new ListItemEdit(rx);
    delete editor;
    // If we reach here without crash, test passes
    QVERIFY(true);
}

void TestListItemEdit::test_createEditor()
{
    QWidget parent;
    QStyleOptionViewItem option;
    QModelIndex index = m_model->index(0, 0);

    QWidget *editor = m_listItemEdit->createEditor(&parent, option, index);
    QVERIFY(editor != nullptr);
    QVERIFY(qobject_cast<QLineEdit *>(editor) != nullptr);

    delete editor;
}

void TestListItemEdit::test_createEditor_WithValidParent()
{
    QWidget parent;
    parent.setGeometry(0, 0, 200, 30);
    QStyleOptionViewItem option;
    option.rect = QRect(0, 0, 200, 30);
    QModelIndex index = m_model->index(0, 0);

    QWidget *editor = m_listItemEdit->createEditor(&parent, option, index);
    QVERIFY(editor != nullptr);
    QVERIFY(editor->parent() == &parent);

    delete editor;
}

void TestListItemEdit::test_setEditorData()
{
    QWidget parent;
    QStyleOptionViewItem option;
    QModelIndex index = m_model->index(0, 0);

    QWidget *editor = m_listItemEdit->createEditor(&parent, option, index);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    QVERIFY(lineEdit != nullptr);

    // Set data in model
    m_model->setData(index, "test_data", Qt::EditRole);

    // Call setEditorData to populate the editor
    m_listItemEdit->setEditorData(editor, index);

    // Verify the editor has the correct data
    QVERIFY(lineEdit->text() == "test_data");

    delete editor;
}

void TestListItemEdit::test_setModelData()
{
    QWidget parent;
    QStyleOptionViewItem option;
    QModelIndex index = m_model->index(0, 0);

    QWidget *editor = m_listItemEdit->createEditor(&parent, option, index);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    QVERIFY(lineEdit != nullptr);

    // Set text in the editor
    lineEdit->setText("updated_data");

    // Call setModelData to update the model
    m_listItemEdit->setModelData(editor, m_model, index);

    // Verify the model has the updated data
    QVERIFY(m_model->data(index, Qt::EditRole).toString() == "updated_data");

    delete editor;
}

void TestListItemEdit::test_updateEditorGeometry()
{
    QWidget parent;
    QStyleOptionViewItem option;
    option.rect = QRect(10, 20, 200, 30);
    QModelIndex index = m_model->index(0, 0);

    QWidget *editor = m_listItemEdit->createEditor(&parent, option, index);

    // Call updateEditorGeometry
    m_listItemEdit->updateEditorGeometry(editor, option, index);

    // Verify the editor geometry matches the option rect
    QVERIFY(editor->geometry() == option.rect);

    delete editor;
}

QTEST_MAIN(TestListItemEdit)
