using System;

namespace lab4_FabricMethod
{
    class Medium: Team
    {
        public Medium(string name) : base(name, 18) {
            Setup();
        }
        public override Player CreatePlayer()
        {
            var random = new Random().Next(0, 10);
            if (random > 8)
                return new Star(this);
            else if (random > 2)
                return new Experienced(this);
            else return new Youngster(this);
        }
    }
}
