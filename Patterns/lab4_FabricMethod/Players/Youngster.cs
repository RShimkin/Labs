using System;

namespace lab4_FabricMethod
{
    class Youngster: Player
    {
        private double GetDif()
        {
            return new Random().NextDouble() * 30 - 20;
        }
        public Youngster(Team team)
        {
            var skdif = GetDif();
            var expdif = GetDif();
            var atdif = GetDif();
            Skill = 50 + skdif;
            Experience = 50 + expdif;
            Athleticism = 50 + atdif;
            Name = $"YoungsterFrom{team.Name}";
            Team = team;
        }
    }
}
