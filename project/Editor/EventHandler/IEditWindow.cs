using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.EventHandler
{
    public interface IEditWindow
    {
        void WindowResizeCallback(Int32 w, Int32 h);
        void WindowMouseMoveCallback(Int32 x, Int32 y);
        void WindowMouseButtonCallback(Int32 button, Boolean pressed, Int32 modkeys);
        void WindowScrollCallback(Int32 scrollX, Int32 scrollY);
        void WindowKeyboardCallback(Int32 key, Boolean pressed, Int32 modkeys);
        void WindowCharacterCallback(Int32 codepoint);
        void WindowFocusCallback(Boolean focus);
    }
}
