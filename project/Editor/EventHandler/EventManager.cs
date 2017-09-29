using System;
using System.Collections.Generic;

namespace Editor.EventHandler
{
    public enum ObjectTypes
    {
        FAKE = 0x0000,
        STATIC = 0x0001,
        ANIM = 0x0002,
        WEAPON = 0x0004,
        AMMO = 0x0008,
        DIALOG = 0x0010,
        SCRIPT = 0x0020,
        DECAL = 0x0040,
        AUDIO = 0x0080,
        SCENE = 0x0100,
        RENDERLAYER = 0x0200,
        STARTUPSCENE = 0x0400,
        ARMOR = 0x0800,
    }

    public class SaveEventArgs
    {
        public bool PackData { get; set; }
    }

    public class QueryDataArgs
    {
        public ObjectTypes ObjectType { get; set; }
        public List<DataSources.BaseData> ReturnList { get; set; }
    }

    public class AddObjectArgs
    {
        public ObjectTypes ObjectType { get; set; }
        public string Name { get; set; }
        public UInt32 FormID { get; set; }
        public object Data { get; set; }
    }

    public class FormArgs
    {
        public ObjectTypes ObjectType { get; set; }
        public UInt32 FormID { get; set; }
        public object Data { get; set; }
    }

    public class GetFormIDArgs
    {
        public UInt32 FormID { get; set; }
    }

    public class FormView
    {
        public int totalEntryCount;
        public List<DataSources.ViewForm> ReturnList { get; set; }
    }

    public enum CloseType
    {
        CLOSE,
        HIDE,
        ABORT
    }

    public class CloseArgs
    {
        public CloseType close;
    }

    public static class EventManager
    {
        public static bool SendRefresEvent = true;

        public static EventHandler<SaveEventArgs> onSaveEvent;
        public static EventHandler<QueryDataArgs> onQueryDataEvent;

        public static EventHandler<AddObjectArgs> onAddObjectEvent;
        public static EventHandler<FormArgs> onDeleteFormEvent;
        public static EventHandler<FormArgs> onEditFormEvent;
        public static EventHandler<GetFormIDArgs> onGetFormIDEvent;

        public static EventHandler<FormView> onGetFormView;

        public static EventHandler<bool> onRefreshFormsEvent;

        public static EventHandler<FormArgs> onSceneChangeEvent;

        public static EventHandler<UInt32> onAddToSceneEvent;

        public static void OnSaveEvent(SaveEventArgs saveArg)
        {
            onSaveEvent?.Invoke(null, saveArg);
        }

        public static void OnQueryDataEvent(QueryDataArgs queryArg)
        {
            onQueryDataEvent?.Invoke(null, queryArg);
        }

        public static void OnAddObjectEvent(AddObjectArgs addArg)
        {
            onAddObjectEvent?.Invoke(null, addArg);
            OnRefreshFormsEvent(SendRefresEvent);
        }

        public static void OnDeleteFormEvent(FormArgs arg)
        {
            onDeleteFormEvent?.Invoke(null, arg);
            OnRefreshFormsEvent(SendRefresEvent);
        }

        public static void OnEditFormEvent(FormArgs arg)
        {
            onEditFormEvent?.Invoke(null, arg);
        }

        public static void OnGetFormIDEvent(GetFormIDArgs arg)
        {
            onGetFormIDEvent?.Invoke(null, arg);
        }

        public static void OnGetFormView(FormView arg)
        {
            onGetFormView?.Invoke(null, arg);
        }

        public static void OnRefreshFormsEvent(bool arg)
        {
            onRefreshFormsEvent?.Invoke(null, arg);
        }

        public static void OnSceneChangeEvent(FormArgs arg)
        {
            onSceneChangeEvent?.Invoke(null, arg);
        }

        public static void OnAddToSceneEvent(UInt32 id)
        {
            onAddToSceneEvent?.Invoke(null, id);
        }
    }
}