using System;

namespace Editor.EditorWindows.AddItemDlgPages
{
    public interface IAddItemBase
    {
        EventHandler.ObjectTypes GetAddType();

        string GetName();

        UInt32 GetFormId();

        object GetItemData();

        bool IsValidData();

        string GetFailMessage();
    }
}