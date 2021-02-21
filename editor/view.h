#pragma once

#include <functional>
#include <forward_list>
#include <memory>
#include <vector>

using namespace std;

// Observer pattern (Pub-Subscribe)
template<typename EventArgs>
class Event{
    public:
        void Subscribe(function<void(EventArgs)>){};
        void NotifyAll();
    private:
        forward_list<function<void(const EventArgs&)>> m_handlers;
};

class EventArgs {};
class MenuItemSelectedArgs: public EventArgs{};

class UserControl{};

class MenuItem: public UserControl{
    public:
        MenuItem(const string& name, const string& caption)
        : Name(name), Caption(caption)
        {}

        string Name;
        string Caption;
        Event<MenuItemSelectedArgs> ItemSelectedEvent; 
};

class Menu: public UserControl{
    public:
        void AddMenuItem(const string& name, const string& caption){};
        void SubscribeOnMenuItemSelected(const string & name, function<void(const MenuItemSelectedArgs&)>){}
    private:
        vector<MenuItem> m_menu_items;
};

class ContextMenu: public Menu{
    public:
        void Show(){};
};

enum MouseButton{
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
        string Message;
};

enum UserChoice{
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
        string GetPath();
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
        shared_ptr<Canvas> ShapesCanvas;

        void Show(){};
}; 