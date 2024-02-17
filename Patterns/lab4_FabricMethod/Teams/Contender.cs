using System;

namespace lab4_FabricMethod
{
    class Contender: Team
    {
        public Contender(string name) : base(name, 23) { 
            Setup();
        }
        public override Player CreatePlayer()
        {
            var random = new Random().Next(0, 10);
            if (random > 2)
                return new Star(this);
            else
                return new Experienced(this);
        }
    }
}
