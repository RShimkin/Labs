using System;

namespace lab4_FabricMethod
{
    class Outsider : Team
    {
        public Outsider(string name): base(name, 14) {
            Setup();
        }
        public override Player CreatePlayer()
        {
            var random = new Random().Next(0, 10);
            if (random > 7)
                return new Experienced(this);
            else return new Youngster(this);
        }
    }
}
