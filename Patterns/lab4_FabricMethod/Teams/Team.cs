using System.Collections.Generic;
using System.Linq;

namespace lab4_FabricMethod
{
    abstract class Team : IPlayerCreator
    {
        static int idgen = 0;
        public int Id { get; set; }

        public string Name { get; set; }

        public List<Player> Players { get; set; }

        public double Average
        {
            get => Players.Average(p => (p.Skill + p.Experience + p.Athleticism) / 3);
        }

        public int Points { get; set; }

        public int Count { get; set; }

        public void Setup()
        {
            for (int i = 0; i < Count; i++)
            {
                Players.Add(CreatePlayer());
            }
        }
        public Team(string name, int playersCount)
        {
            Id = ++idgen;
            Count = playersCount;
            Name = name;
            Players = new List<Player>();
        }

        public abstract Player CreatePlayer();

        public override string ToString()
        {
            return $"Team #{Id} {Name} av:{Average} pts:{Points}";
        }

        public IEnumerable<string> PrintPlayers()
        {
            foreach (var x in Players)
            {
                yield return x.ToString();
            }
        }
    }
}
