using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab4_FabricMethod
{
    class Championship
    {
        public string Name { get; set; }

        public Team Winner { get; set; }

        public IList<Team> Teams { get; set; }

        public IList<Match> Matches { get; set; }

        public int RoundsCount { get; set; }

        public Championship(int rounds = 1)
        {
            RoundsCount = rounds;
            Matches = new List<Match>();
            Teams = new List<Team>();
            Teams.Add(new Medium("Everton"));
            Teams.Add(new Medium("Sevilla"));
            Teams.Add(new Outsider("Spartak"));
            Teams.Add(new Contender("BayernMunich"));
        }

        public void Play(bool showPlayers = true)
        {
            int num = Teams.Count;
            for (int r = 0; r < RoundsCount; r++)
            {
                for (int i = 0; i < num - 1; i++)
                {
                    for (int k = i + 1; k < num; k++)
                    {
                        Match match = new(Teams[i], Teams[k]);
                        match.Play();
                        match.Print(showPlayers);
                        Matches.Add(match);
                    }
                }
            }

            Teams.OrderBy(x => x.Points);
            Winner = Teams.Last();
            Console.WriteLine("\nWinner:");
            Console.WriteLine(Winner.ToString());
        }
    }
}
