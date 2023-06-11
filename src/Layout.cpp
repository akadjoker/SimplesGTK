#include "Layout.hpp"

void Layout::Add(Widget *widget)
{
    gtk_container_add(m_container, widget->m_widget);
    widget->OnAdd();
}

void Layout::Add(Layout *widget)
{
    gtk_container_add(m_container, widget->m_widget);
    widget->OnAdd();
}

void Layout::addWidget(GtkWidget *widget)
{
    gtk_container_add(GTK_CONTAINER(m_widget), widget);
}

Button *Layout::GetButtonByID(const std::string &id)
{
    try
    {

        for (auto &button : m_childs)
        {
            if (button->GetId() == id)
            {
                Button *btn = dynamic_cast<Button *>(button.get());
                return btn;
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error : " << ex.what() << std::endl;
    }

    g_print("Button %s don't exists .\n", id.c_str());
    return nullptr;
}

TextEdit *Layout::GetTextEditByID(const std::string &id)
{
    try
    {
        for (auto &textEdit : m_childs)
        {
            if (textEdit->GetId() == id)
            {
                TextEdit *txt = dynamic_cast<TextEdit *>(textEdit.get());
                return txt;
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error : " << ex.what() << std::endl;
    }

    g_print("TextEdit %s don't exists .\n", id.c_str());
    return nullptr;
}

Widget *Layout::GetWidgetByID(const std::string &id)
{
    try
    {

        for (auto &widget : m_childs)
        {
            if (widget->GetId() == id)
            {
                return widget.get();
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error : " << ex.what() << std::endl;
    }

    g_print("Widget %s don't exists .\n", id.c_str());
    return nullptr;
}

Button *Layout::CreateButton(const std::string &text, const std::string &id)
{
    std::shared_ptr<Button> button = std::make_shared<Button>(text);
    button.get()->SetId(id);
    m_childs.push_back(button);
    button->m_mainWindow = m_mainWindow;
    return button.get();
}

CheckButton *Layout::CreateCheckButton(const std::string &text, const std::string &id)
{

    std::shared_ptr<CheckButton> button = std::make_shared<CheckButton>(text);
    button.get()->SetId(id);
    m_childs.push_back(button);
    button->m_mainWindow = m_mainWindow;
    return button.get();
}

RadioButton *Layout::CreateRadioButton(const std::string &text, const std::string &id)
{

    std::shared_ptr<RadioButton> button = std::make_shared<RadioButton>(text);
    button.get()->SetId(id);
    m_childs.push_back(button);
    button->m_mainWindow = m_mainWindow;
    return button.get();
}

ToggleButton *Layout::CreateToggleButton(const std::string &text, const std::string &id)
{

    std::shared_ptr<ToggleButton> button = std::make_shared<ToggleButton>(text);
    button.get()->SetId(id);
    m_childs.push_back(button);
    button->m_mainWindow = m_mainWindow;
    return button.get();
}

Label *Layout::CreateLabel(const std::string &text, const std::string &ID)
{
    std::shared_ptr<Label> label = std::make_shared<Label>(text);
    label.get()->SetId(ID);
    m_childs.push_back(label);
    label->m_mainWindow = m_mainWindow;
    return label.get();
}

TextEdit *Layout::CreateTextEdit(const std::string &text, const std::string &ID)
{

    std::shared_ptr<TextEdit> textedit = std::make_shared<TextEdit>();
    textedit.get()->SetId(ID);
    textedit.get()->SetText(text);
    m_childs.push_back(textedit);
    textedit->m_mainWindow = m_mainWindow;
    return textedit.get();
}

TextView *Layout::CreateTextView(const std::string &text, const std::string &ID)
{

    std::shared_ptr<TextView> textview = std::make_shared<TextView>();
    textview.get()->SetId(ID);
    textview.get()->AddText(text);
    m_childs.push_back(textview);
    textview->m_mainWindow = m_mainWindow;
    return textview.get();
}

ListBox *Layout::CreateListBox(const std::string &ID)
{

    std::shared_ptr<ListBox> listbox = std::make_shared<ListBox>();
    listbox.get()->SetId(ID);
    m_childs.push_back(listbox);
    listbox->m_mainWindow = m_mainWindow;
    return listbox.get();
}

ComboBox *Layout::CreateComboBox(const std::string &ID)
{

    std::shared_ptr<ComboBox> combobox = std::make_shared<ComboBox>();
    combobox.get()->SetId(ID);
    m_childs.push_back(combobox);
    combobox->m_mainWindow = m_mainWindow;
    return combobox.get();
}

ComboBoxText *Layout::CreateComboBoxText(bool hasEntry, const std::string &ID)
{

    std::shared_ptr<ComboBoxText> comboboxtext = std::make_shared<ComboBoxText>(hasEntry);
    comboboxtext.get()->SetId(ID);
    m_childs.push_back(comboboxtext);
    comboboxtext->m_mainWindow = m_mainWindow;
    return comboboxtext.get();
}

MenuBar *Layout::CreateMenuBar(const std::string &ID)
{

    std::shared_ptr<MenuBar> menubar = std::make_shared<MenuBar>();
    menubar.get()->SetId(ID);
    m_childs.push_back(menubar);
    menubar->m_mainWindow = m_mainWindow;
    return menubar.get();
}

ToolBar *Layout::CreateToolBar(ToolBarStyle style, const std::string &ID)
{
    std::shared_ptr<ToolBar> toolbar = std::make_shared<ToolBar>(style);
    toolbar.get()->SetId(ID);
    m_childs.push_back(toolbar);
    toolbar->m_mainWindow = m_mainWindow;
    return toolbar.get();
}

Menu *Layout::CreateMenu(const std::string &ID)
{

    std::shared_ptr<Menu> menu = std::make_shared<Menu>();
    menu.get()->SetId(ID);
    m_childs.push_back(menu);
    menu->m_mainWindow = m_mainWindow;
    return menu.get();
}

ProgressBar *Layout::CreateProgressBar(const std::string &ID)
{

    std::shared_ptr<ProgressBar> progressbar = std::make_shared<ProgressBar>();
    progressbar.get()->SetId(ID);
    m_childs.push_back(progressbar);
    progressbar->m_mainWindow = m_mainWindow;
    return progressbar.get();
}

LevelBar *Layout::CreateLevelBar(const std::string &ID)
{

    std::shared_ptr<LevelBar> levelbar = std::make_shared<LevelBar>();
    levelbar.get()->SetId(ID);
    m_childs.push_back(levelbar);
    levelbar->m_mainWindow = m_mainWindow;
    return levelbar.get();
}

LevelBar *Layout::CreateLevelBar(double min_value, double max_value, const std::string &ID)
{

    std::shared_ptr<LevelBar> levelbar = std::make_shared<LevelBar>(min_value, max_value);
    levelbar.get()->SetId(ID);
    m_childs.push_back(levelbar);
    levelbar->m_mainWindow = m_mainWindow;
    return levelbar.get();
}
//********************************************************************************
// FixedLayout
//********************************************************************************

FrameLayout::FrameLayout(const std::string &title)
{
    m_widget = gtk_frame_new(title.c_str());
    m_frame = GTK_FRAME(m_widget);
    m_container = GTK_CONTAINER(m_widget);
}

// void FrameLayout::Add(Widget *widget)
// {
//     gtk_container_add(m_container, widget->m_widget);
//     widget->OnAdd();
// }

// void FrameLayout::Add(Layout *widget)
// {
//     gtk_container_add(m_container, widget->m_widget);
//     widget->OnAdd();
// }

BoxLayout::BoxLayout(Orientation orientation, int spacing)
{
    if (orientation == Orientation::Vertical)
    {
        m_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    }
    else
    {
        m_widget = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);
    }
    m_box = GTK_BOX(m_widget);
}

void BoxLayout::PackStart(Widget *widget, bool expand, bool fill, int padding)
{
    gtk_box_pack_start(m_box, widget->m_widget, expand, fill, padding);
}
void BoxLayout::packStart(GtkWidget *widget, bool expand, bool fill, int padding)
{
    gtk_box_pack_start(m_box, widget, expand, fill, padding);
}

void BoxLayout::PackEnd(Widget *widget, bool expand, bool fill, int padding)
{
    gtk_box_pack_end(m_box, widget->m_widget, expand, fill, padding);
}
void BoxLayout::packEnd(GtkWidget *widget, bool expand, bool fill, int padding)
{
    gtk_box_pack_end(m_box, widget, expand, fill, padding);
}

gboolean on_fixed_drag_drop(GtkWidget *widget, GdkDragContext *context,
                            gint x, gint y, guint time, gpointer user_data)
{
    if (context)
    {
        GdkDragAction action = gdk_drag_context_get_actions(context);
        if (action == GDK_ACTION_MOVE)
        {
            GtkWidget *source_widget = gtk_drag_get_source_widget(context);
            if (GTK_IS_WIDGET(source_widget))
            {
                // gint width = gtk_widget_get_allocated_width(source_widget);
                // gint height = gtk_widget_get_allocated_height(source_widget);
                // gtk_fixed_move(GTK_FIXED(widget), source_widget, x - width / 2, y - height / 2);
                gtk_fixed_move(GTK_FIXED(widget), source_widget, x, y);
            }
        }
    }

    return TRUE;
}

FixedLayout::FixedLayout()
{
    m_widget = gtk_fixed_new();
    m_fixed = GTK_FIXED(m_widget);
    m_container = GTK_CONTAINER(m_widget);

    // GtkWidget *combo_box = gtk_combo_box_new();

    // GtkListStore *list_store = gtk_list_store_new(1, G_TYPE_STRING);

    // GtkTreeIter iter;
    // gtk_list_store_append(list_store, &iter);
    // gtk_list_store_set(list_store, &iter, 0, "Opção 1", -1);
    // gtk_list_store_append(list_store, &iter);
    // gtk_list_store_set(list_store, &iter, 0, "Opção 2", -1);
    // gtk_list_store_append(list_store, &iter);
    // gtk_list_store_set(list_store, &iter, 0, "Opção 3", -1);

    // gtk_combo_box_set_model(GTK_COMBO_BOX(combo_box), GTK_TREE_MODEL(list_store));

    // GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    // gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
    // gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "text", 0, NULL);

    //  GtkWidget *combo_box = gtk_combo_box_text_new_with_entry();

    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Opção 1");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Opção 2");
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "Opção 3");

    //  gtk_fixed_put(m_fixed, combo_box, 100,100);

    //     GtkWidget *list_box = gtk_list_box_new();

    //     GtkWidget *label1 = gtk_label_new("Item 1");
    //     GtkWidget *label2 = gtk_label_new("Item 2");
    //     GtkWidget *label3 = gtk_label_new("Item 3");

    //     gtk_list_box_insert(GTK_LIST_BOX(list_box), label1, -1);
    //     gtk_list_box_insert(GTK_LIST_BOX(list_box), label2, -1);
    //     gtk_list_box_insert(GTK_LIST_BOX(list_box), label3, -1);

    //     GtkWidget *button = gtk_button_new_with_label("Button");
    //     GtkWidget *label4 = gtk_label_new("Item 1");

    //     GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    //     gtk_box_pack_start(GTK_BOX(box), label4, TRUE, TRUE, 4);

    //     gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 4);

    //     gtk_list_box_insert(GTK_LIST_BOX(list_box), box, -1);

    //  gtk_fixed_put(m_fixed, list_box, 40,40);
    //  gtk_widget_set_size_request(list_box,180,200);
}

void FixedLayout::Add(Widget *widget, int x, int y)
{
    gtk_fixed_put(m_fixed, widget->m_widget, x, y);
    widget->OnAdd();
}

void FixedLayout::Add(Widget *widget, int x, int y, int width, int height)
{
    assert(widget->m_widget);
    gtk_fixed_put(m_fixed, widget->m_widget, x, y);
    gtk_widget_set_size_request(widget->m_widget, width, height);
    widget->OnAdd();
}

void FixedLayout::SetCandDrag(bool draggable, int button)
{

    GdkDragAction actions = (GdkDragAction)(GDK_ACTION_MOVE);

    if (draggable && drag_id == 0)
    {
        GtkTargetEntry targets[] =
            {
                {(gchar *)"STRING", 0, 0}};

        gtk_drag_dest_set(m_widget, GTK_DEST_DEFAULT_ALL, targets, sizeof(targets) / sizeof(GtkTargetEntry), actions);
        drag_id = g_signal_connect(m_widget, "drag-drop", G_CALLBACK(on_fixed_drag_drop), this);
    }
    else
    {
        if (drag_id != 0)
        {
            g_signal_handler_disconnect(m_widget, drag_id);
            gtk_drag_dest_unset(m_widget);
            drag_id = 0;
        }
    }
}

//************************************************************************
// GridLayout
//************************************************************************

GridLayout::GridLayout()
{

    m_widget = gtk_grid_new();
    m_grid = GTK_GRID(m_widget);
    m_container = GTK_CONTAINER(m_widget);
}

void GridLayout::Add(Widget *widget, int left, int top, int width, int height)
{
    gtk_grid_attach(m_grid, widget->m_widget, left, top, width, height);
    widget->OnAdd();
}

void Layout::DoAdded()
{
}

void Layout::DoRemoved()
{
}

void GridLayout::SetRowSpacing(int spacing)
{
    gtk_grid_set_row_spacing(m_grid, spacing);
}

void GridLayout::SetColumnSpacing(int spacing)
{
    gtk_grid_set_column_spacing(m_grid, spacing);
}

void GridLayout::SetRowHomogeneous(bool homogeneous)
{

    gtk_grid_set_row_homogeneous(m_grid, homogeneous);
}

void GridLayout::SetColumnHomogeneous(bool homogeneous)
{
    gtk_grid_set_column_homogeneous(m_grid, homogeneous);
}

void Layout::SetBoder(int border)
{
    assert(m_widget != nullptr);
    assert(m_container != nullptr);
    gtk_container_set_border_width(m_container, border);
}

bool Layout::Contains(const std::string &id) const
{
    int index = 0;
    for (auto &widget : m_childs)
    {
        if (widget->GetId() == id)
        {
            return true;
        }
        // std::cout<<widget->GetId()<< " - " << index <<std::endl;
        index++;
    }

    return false;
}

void Layout::SetBorder(int border)
{
    gtk_container_set_border_width(m_container, border);
}

Layout *Layout::GetLayoutByID(const std::string &id)
{
    try
    {

        for (auto &layout : m_childs)
        {
            if (layout->GetId() == id)
            {
                Layout *btn = dynamic_cast<Layout *>(layout.get());
                return btn;
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error : " << ex.what() << std::endl;
    }
    return nullptr;
}

//********************************************************************************
//  ScrollLayout
//********************************************************************************

ScrollLayout::ScrollLayout()
{
    m_widget = gtk_scrolled_window_new(NULL, NULL);
    m_scroll = GTK_SCROLLED_WINDOW(m_widget);
    m_container = GTK_CONTAINER(m_widget);

    gtk_scrolled_window_set_policy(m_scroll, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_widget_set_vexpand(m_widget, TRUE);
    gtk_widget_set_hexpand(m_widget, TRUE);
}

//********************************************************************************
//  ViewPortLayout
//********************************************************************************

ViewPortLayout::ViewPortLayout()
{

    m_widget = gtk_viewport_new(NULL, NULL);
    m_viewport = GTK_VIEWPORT(m_widget);
    m_container = GTK_CONTAINER(m_widget);
}

Layout::~Layout()
{
    m_childs.clear();
    //  std::cout << "Remove Layout: " << m_id << std::endl;
}

//********************************************************************************
//  Notebook
//********************************************************************************



Notebook::Notebook()
{
    m_widget = gtk_notebook_new();
    m_notebook = GTK_NOTEBOOK(m_widget);
    
    
}

Notebook::~Notebook()
{
}

int Notebook::AppendPage(Widget *widget, const std::string &label)
{
    return gtk_notebook_append_page(m_notebook, widget->m_widget, gtk_label_new(label.c_str()));
}


