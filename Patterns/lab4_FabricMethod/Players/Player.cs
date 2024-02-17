using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab4_FabricMethod
{
    class Player
    {
        private static int idgen = 0;
        public int Id { get; set; }

        public string Name { get; set; }

        public double Skill { get; set; }

        public double Athleticism { get; set; }

        public double Experience { get; set; }

        public Team Team { get; set; }

        public Player()
        {
            Id = ++idgen;
        }

        public override string ToString()
        {
            return $"Player #{Id} {Name} sk:{(int)Skill} at:{(int)Athleticism} exp:{(int)Experience}";
        }
    }
}
