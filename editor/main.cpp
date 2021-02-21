#include "controller.h"
#include "view.h"


void OnNewDocumentCreation(Editor& editor, const MenuItemSelectedArgs& eventArgs)
{
    editor.CreateNewDocument();
}

void OnDocumentImport(Editor& editor, const MenuItemSelectedArgs& eventArgs)
{
    editor.ImportDocument();
}

void OnDocumentExport(Editor& editor, const MenuItemSelectedArgs& eventArgs)
{
    editor.ExportDocument();
}

void OnShapeCreation(Editor& editor, const MenuItemSelectedArgs& eventArgs)
{
    GetShapeDetailsDialog shapesDetailsDialog;
    // ...

    shared_ptr<Shape> circle(new Circle());
    editor.AddShape(circle);
}

void OnCanvasClick(Editor& editor, const ClickEventArgs& eventArgs)
{
    if (eventArgs.ButtonType == MouseButton::Left){
        editor.SelectShapeClicked(eventArgs.X, eventArgs.Y);
    }
    else{
        editor.Form.PopupMenu.Show();
    }
}

void OnShapeErasing(Editor& editor, const MenuItemSelectedArgs& eventArgs)
{
    editor.EraseActiveShape();
}

int main (int, char **)
{
    Editor editor;
    auto barMenu = editor.Form.MenuBar;

    barMenu.SubscribeOnMenuItemSelected(
        "NewDocument",
        [&editor](const MenuItemSelectedArgs& eventArgs){OnNewDocumentCreation(editor, eventArgs);});
    barMenu.SubscribeOnMenuItemSelected(
        "Import",
        [&editor](const MenuItemSelectedArgs& eventArgs){OnDocumentImport(editor, eventArgs);});
    barMenu.SubscribeOnMenuItemSelected(
        "Export",
        [&editor](const MenuItemSelectedArgs& eventArgs){OnDocumentExport(editor, eventArgs);});

    auto popupMenu = editor.Form.PopupMenu;
    popupMenu.SubscribeOnMenuItemSelected(
        "AddShape",
        [&editor](const MenuItemSelectedArgs& eventArgs){OnShapeCreation(editor, eventArgs);});

    popupMenu.SubscribeOnMenuItemSelected(
        "EraseShape",
        [&editor](const MenuItemSelectedArgs& eventArgs){OnShapeErasing(editor, eventArgs);});

    auto canvas = editor.Form.ShapesCanvas;
    canvas->CanvasClickedEvent.Subscribe(
        [&editor](const ClickEventArgs& eventArgs){OnCanvasClick(editor, eventArgs);});

    editor.Form.Show();

    //MenuItemSelectedArgs args;
    //OnNewDocumentCreation(editor, args);
    //OnShapeCreation(editor, args);

    return 0;
}