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
		public string FileName { get; set; }
		public bool PackData { get; set; }
	}

    public class QueryDataArgs
    {
        public ObjectTypes ObjectType { get; set; }
        public List<object> returnList { get; set; }
    }

	public static class EventManager
	{

		public static EventHandler<SaveEventArgs> onSaveEvent;
        public static EventHandler<QueryDataArgs> onQueryDataEvent;

		public static void OnSaveEvent(SaveEventArgs saveArgs)
		{
			onSaveEvent?.Invoke(null, saveArgs);
		}

        public static void OnQueryDataEvent(QueryDataArgs queryArgs)
        {
            onQueryDataEvent?.Invoke(null, queryArgs);
        }

	}
}
