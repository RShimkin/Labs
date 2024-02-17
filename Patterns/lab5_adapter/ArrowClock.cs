using System.Threading;

namespace lab5_adapter
{
    class ArrowClock
    {
        public int Alpha { get; set; }

        public int Beta { get; set; }

        public int Gamma { get; set; }

        private int speed;

        Timer timer;

        private static void TimerFunction(object obj)
        {
            (obj as ArrowClock).GoAhead();
        }

        private void GoAhead()
        {
            Gamma += 6 * speed;
            if (Gamma > 360)
            {
                Beta += (Gamma / 360) * 6;
                if (Beta > 360)
                {
                    Alpha += (Beta / 360) * 30;
                    Beta %= 360;
                }
                Gamma %= 360;
            }
        }

        public ArrowClock(int a, int b, int g, int speed = 1)
        {
            (Alpha, Beta, Gamma) = (a, b, g);
            this.speed = speed >= 1 ? speed : 1;
            timer = new(new TimerCallback(TimerFunction), this, 0, 1000);
        }

        public int RollAhead(Period period)
        {
            switch (period)
            {
                case Period.HOUR:
                    Alpha += 30;
                    Alpha %= 360;
                    break;
                case Period.MINUTE:
                    Beta += 6;
                    //Alpha += (Alpha / 30) * 30 + (int)((Beta / 360f) * 30);
                    
                    if (Beta >= 360)
                    {
                        Alpha = (Alpha + 30) % 360;
                        Beta %= 360;
                    }
                    
                    break;
                case Period.SECOND:
                    Gamma += 6;
                    if (Gamma >= 360)
                    {
                        Beta += (Gamma / 360) * 6;
                        if (Beta >= 360)
                        {
                            Alpha += (Beta / 360) * 30;
                            Beta %= 360;
                        }
                        Gamma %= 360;
                    }
                    break;
            }
            return Alpha;
        }

        public int RollBack(Period period)
        {
            switch (period)
            {
                case Period.HOUR:
                    Alpha -= 30;
                    if (Alpha < 0) Alpha += 360;
                    break;
                case Period.MINUTE:
                    Beta -= 6;
                    if (Beta < 0)
                    {
                        Alpha -= 30;
                        if (Alpha < 0) Alpha += 360;
                        Beta += 360;
                    }
                    break;
                case Period.SECOND:
                    Gamma -= 6;
                    if (Gamma < 0)
                    {
                        Beta -= 30;
                        if (Beta < 0)
                        {
                            Alpha -= 30;
                            if (Alpha < 0) Alpha += 360;
                            Beta += 360;
                        }
                        Gamma += 360;
                    }
                    break;
            }
            return Alpha;
        }

        public void SetTime(int alpha, int beta, int gamma)
        {
            Alpha = alpha;
            Beta = beta;
            Gamma = gamma;
        }

        public (int, int, int) GetAngles()
        {
            return (Alpha, Beta, Gamma);
        }

        public override string ToString()
        {
            return $"{Alpha} {Beta} {Gamma}";
        }
    }
}
