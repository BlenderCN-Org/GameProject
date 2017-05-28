using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
