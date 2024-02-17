using System;

namespace lab4_FabricMethod
{
    class Experienced: Player
    {
        private double GetDif()
        {
            return new Random().NextDouble() * 30 - 15;
        }
        public Experienced(Team team)
        {
            Skill = 75 + GetDif();
            Experience = 75 + GetDif();
            Athleticism = 75 + GetDif();
            Name = $"ExperiencedFrom{team.Name}";
            Team = team;
        }
    }
}
