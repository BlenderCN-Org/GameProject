using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.EventHandler
{

	public class SaveEventArgs
	{
		public string FileName { get; set; }
		public bool PackData { get; set; }
	}

	public static class EventManager
	{

		public static EventHandler<SaveEventArgs> onSaveEvent;

		public static void OnSaveEvent(SaveEventArgs saveArgs)
		{
			onSaveEvent?.Invoke(null, saveArgs);
		}
	}
}
