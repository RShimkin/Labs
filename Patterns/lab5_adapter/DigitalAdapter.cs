using System;
using System.Collections.Generic;
using System.Linq;

namespace lab5_adapter
{
    class DigitalAdapter
    {
        bool show = true;

        bool day = false;

        ArrowClock ac;

        ArrowClock Clock { get
            {
                if (ac == null) ac = new(0, 0, 0);
                return ac;
            }
        }

        public void SetTime(string str)
        {
            if (Clock == null) return;
            int h, m, s;
            List<int> arr = new();
            try
            {
                arr = str.Split(':').Select(x => int.Parse(x)).ToList();
            } catch (Exception e)
            {
                Console.WriteLine("Неправильное время!" + e.Message);
            }
            (h, m, s) = (arr[0], arr[1], arr[2]);
            h = h - 12;
            int alpha = (int)((h / 12f) * 360);
            int beta = (int)((m / 60f) * 360);
            int gamma = (int)((s / 60f) * 360);
            Clock.SetTime(alpha, beta, gamma);
            Console.WriteLine(Clock.ToString());
        }

        public void AddHour()
        {
            int newh, oldh;
            (oldh, _, _) = Clock.GetAngles();
            newh = Clock.RollAhead(Period.HOUR);
            if (newh < oldh)
            {
                day = !day;
            }
        }

        public void SubHour()
        {
            int newh, oldh;
            (oldh, _, _) = Clock.GetAngles();
            newh = Clock.RollBack(Period.HOUR);
            if (newh > oldh)
            {
                day = !day;
            }
        }

        public void AddMinute()
        {
            int newh, oldh;
            (oldh, _, _) = Clock.GetAngles();
            newh = Clock.RollAhead(Period.MINUTE);
        }

        public void SubMinute()
        {
            int newh, oldh;
            (oldh, _, _) = Clock.GetAngles();
            newh = Clock.RollBack(Period.MINUTE);
            if (newh > oldh)
            {
                day = !day;
            }
        }

        public void AddSecond()
        {
            int newh, oldh;
            (oldh, _, _) = Clock.GetAngles();
            newh = Clock.RollAhead(Period.SECOND);
        }

        public void SubSecond()
        {
            int newh, oldh;
            (oldh, _, _) = Clock.GetAngles();
            newh = Clock.RollBack(Period.SECOND);
            if (newh > oldh)
            {
                day = !day;
            }
        }

        public string GetTime()
        {
            var tuple = Clock.GetAngles();
            int alpha = tuple.Item1, beta = tuple.Item2, gamma = tuple.Item3;
            int s = (int)((gamma / 360f) * 60);
            int m = (int)((beta / 360f) * 60);
            int h = (int)((alpha / 360f) * 12);
            if (day) h += 12;
            //return $"{h}:{m}:{s}";
            return $"{h}:{m}:{s}";
        }

        public override string ToString()
        {
            if (show)
                return $"{Clock.ToString()} | {GetTime()}";
            return GetTime();
        }
    }
}
