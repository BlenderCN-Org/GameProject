using System;
using System.Collections.Generic;

namespace Editor.EventHandler
{

    public enum ObjectTypes{
        STATIC = 0x0001,
        ANIM   = 0x0002,
        WEAPON = 0x0004,
        AMMO   = 0x0008,
        DIALOG = 0x0010,
        SCRIPT = 0x0020,
        DECAL  = 0x0040,
        AUDIO  = 0x0080,
        SCENE  = 0x0100
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
        public string FormID { get; set; }
        public object Data { get; set; }
    }

    public class FormArgs
    {
        public UInt32 FormID { get; set; }
        public object Data { get; set; }
    }

	public static class EventManager
	{

		public static EventHandler<SaveEventArgs> onSaveEvent;
        public static EventHandler<QueryDataArgs> onQueryDataEvent;
        public static EventHandler<AddObjectArgs> onAddObjectEvent;

        public static EventHandler<FormArgs> onAddFormEvent;
        public static EventHandler<FormArgs> onDeleteFormEvent;
        public static EventHandler<FormArgs> onEditFormEvent;
        

        public static void OnSaveEvent(SaveEventArgs saveArgs)
		{
			onSaveEvent?.Invoke(null, saveArgs);
		}

        public static void OnQueryDataEvent(QueryDataArgs queryArgs)
        {
            onQueryDataEvent?.Invoke(null, queryArgs);
        }

        public static void OnAddObjectEvent(AddObjectArgs addArgs)
        {
            onAddObjectEvent?.Invoke(null, addArgs);
        }

        public static void OnAddFormEvent(FormArgs args)
        {
            onAddFormEvent?.Invoke(null, args);
        }

        public static void OnDeleteFormEvent(FormArgs args)
        {
            onDeleteFormEvent?.Invoke(null, args);
        }

        public static void OnEditFormEvent(FormArgs args)
        {
            onEditFormEvent?.Invoke(null, args);
        }

    }
}
