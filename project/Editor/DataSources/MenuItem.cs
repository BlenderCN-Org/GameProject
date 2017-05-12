using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.DataSources
{
    public class MenuItem : BaseData
    {
        public MenuItem()
        {

        }

        public MenuItem(uint eid, string name) : base(eid, name)
        {
        }

        public new string Name { get; set; }

        public string Command { get; set; }
        

    }
}
