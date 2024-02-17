using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab4_FabricMethod
{
    class Match
    {
        private static int idgen = 0;

        private static DateTime dtgen = DateTime.Now;
        public int Id { get; set; }

        private bool Complete { get; set; } = false;
        public DateTime Date { get; set; }

        public string Location { get; set; }

        public (int, int) Score { get; set; }

        public Team Winner { get; set; }

        public Team Team1 { get; set; }
        public Team Team2 { get; set; }

        public List<Player> Players { get; set; }

        public Match(Team first, Team second)
        {
            Id = ++idgen;
            Date = dtgen.AddDays(3).AddHours(8);
            Team1 = first;
            Team2 = second;
        }

        public void Play()
        {
            double avg1 = Team1.Average, avg2 = Team2.Average;
            double koef = avg1 / avg2;
            int min = new Random().Next(5);
            double dif = new Random().NextDouble() - 0.5;
            if (koef < 1)
            {
                Score = (min, ((int)Math.Floor(min / koef + dif)));
            }
            else
            {
                Score = (((int)Math.Ceiling(min * koef + dif)), min);
            }
            int s1, s2;
            (s1,s2) = Score;
            if (s1 > s2)
            {
                Winner = Team1;
                Team1.Points += 3;
            }
            else if (s1 < s2)
            {
                Winner = Team2;
                Team2.Points += 3;
            }
            else
            {
                Winner = null;
                Team1.Points += 1;
                Team2.Points += 1;
            }
            Complete = true;
        }

        public string ToString()
        {
            if (!Complete)
            {
                return $"Match {Team1.Name} vs {Team2.Name} awaiting";
            }
            int s1, s2;
            (s1, s2) = Score;
            return $"Match {Date.ToShortDateString()} {Team1.Name} vs {Team2.Name} {s1}-{s2}";
        }

        public void Print(bool verbose = true)
        {
            Console.WriteLine("\n==============================================");
            Console.WriteLine(ToString());
            foreach (var team in new List<Team>() { Team1, Team2})
            {
                Console.WriteLine(team.ToString());
                if (verbose)
                {
                    foreach (var player in team.PrintPlayers())
                    {
                        Console.WriteLine(player);
                    }
                }
            }
        }
    }
}
