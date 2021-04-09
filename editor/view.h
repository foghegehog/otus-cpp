#pragma once

#include <functional>
#include <forward_list>
#include <memory>
#include <vector>

// Observer pattern (Pub-Subscribe)
template<typename EventArgs>
class Event{
    public:
        void Subscribe(std::function<void(EventArgs)>){};
        void NotifyAll();
    private:
        std::forward_list<std::function<void(const EventArgs&)>> m_handlers;
};

class EventArgs {};
class MenuItemSelectedArgs: public EventArgs{};

class UserControl{};

class MenuItem: public UserControl{
    public:
        MenuItem(const std::string& name, const std::string& caption)
        : Name(name), Caption(caption)
        {}

        std::string Name;
        std::string Caption;
        Event<MenuItemSelectedArgs> ItemSelectedEvent; 
};

class Menu: public UserControl{
    public:
        void AddMenuItem(const std::string& name, const std::string& caption){};
        void SubscribeOnMenuItemSelected(const std::string & name, std::function<void(const MenuItemSelectedArgs&)>){}
    private:
        std::vector<MenuItem> m_menu_items;
};

class ContextMenu: public Menu{
    public:
        void Show(){};
};

enum class MouseButton{
    Left,
    Right
};

class ClickEventArgs : public EventArgs
{
    public:
        int X;
        int Y;
        MouseButton ButtonType;
};

class Canvas: public UserControl{
    public:
        Event<ClickEventArgs> CanvasClickedEvent;
};

class Dialog : public UserControl{
    public:
        std::string Message;
};

enum class UserChoice{
    Yes,
    No,
    Cancel
};

class ConfirmationDialog: public Dialog{
    public:
        UserChoice GetUserChoice();
};

class GetPathDialog: public Dialog{
    public:
        std::string GetPath();
};

struct ShapeDetails {};

class GetShapeDetailsDialog: public Dialog{
    public:
        ShapeDetails GetShapeDetails();
};

class EditorForm : public UserControl{
    public:
        EditorForm()
        {
            MenuBar.AddMenuItem("NewDocument", "Create New");
            MenuBar.AddMenuItem("Export", "Export Document");
            MenuBar.AddMenuItem("Import", "Import Document");
            MenuBar.AddMenuItem("AddShape", "Add Shape");
            PopupMenu.AddMenuItem("EraseShape", "Erase Shape");
        }

        Menu MenuBar;
        ContextMenu PopupMenu;
        std::shared_ptr<Canvas> ShapesCanvas;

        void Show(){};
}; 