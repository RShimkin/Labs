using System;

namespace lab4_FabricMethod
{
    class Star: Player
    {
        private double GetDif()
        {
            return new Random().NextDouble() * 10 - 5;
        }
        public Star(Team team)
        {
            Skill = 95 + GetDif();
            Experience = 95 + GetDif();
            Athleticism = 95 + GetDif();
            Name = $"StarFrom{team.Name}";
            Team = team;
        }
    }
}
