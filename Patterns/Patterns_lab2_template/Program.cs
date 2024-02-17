using System;
using System.Text;

namespace Pattern_lab2_template
{
    public abstract class Salad
    {
        public void Recipe()
        {
            PrepareIngredients();
            ThermalOperations();
            Cut();
            Mix();
            Layout();
        }

        protected virtual void PrepareIngredients() { }

        protected virtual void ThermalOperations() { }

        protected virtual void Cut() { }

        protected virtual void Mix() { }

        protected virtual void Layout() { }
    }

    public class Caesar: Salad
    {
        protected override void Layout()
        {
            Console.WriteLine("\tplace the salad on a broad plate");
        }
        protected override void Mix()
        {
            StringBuilder sb = new();
            sb.AppendLine("\nMixing");
            sb.Append("\ttoss lettuce, croutons and dressing\n\tAdd shaved Parmesan\n");
            Console.WriteLine(sb);
        }
        protected override void Cut()
        {
            //base.Cutting();
            StringBuilder sb = new();
            sb.AppendLine("\nCutting");
            sb.Append("  (dressing)\n");
            sb.Append("\tChop together anchovy fillets and garlic\n\tmash into a paste\n\tadd egg yolks, vinegar, mustard\n\twhisk\n");
            Console.WriteLine(sb);
            sb.Clear();
            Console.Write("\tneed more salt and pepper? y/n ");
            var resp = Console.ReadLine();
            if (resp.ToLower().StartsWith('y'))
            {
                sb.Append("\tAdd more salt & pepper\n");
            }
            Console.WriteLine(sb);
        }
        protected override void ThermalOperations()
        {
            StringBuilder sb = new();
            sb.AppendLine("\nBoiling/Baking/Frying");
            sb.Append("\n\tPreheat oven 375\n\tToss bread with olive oil\n\tAdd salt & pepper\n\tBake 10-15 minutes\n");
            Console.WriteLine(sb);
        }
        protected override void PrepareIngredients()
        {
            StringBuilder sb = new();
            sb.AppendLine("list of Caesar ingredients:");
            sb.AppendLine("  (dressing)");
            sb.Append("\tParmesan\n\tanchovy\n\teggs\n\tvinegar or lemon juice\n\tmustard\n\tgarlic\n\tsalt & pepper \n\tWorcestershire sauce\n\tmayonnaise\n");
            sb.AppendLine("  (salad)");
            sb.Append("\tRomaine lettuce\n\tParmesan\n\tcroutons\n");
            sb.Append("  (croutons)");
            sb.Append("\tbutter\n\tolive oil\n\t(italian?) bread\n\tsalt & pepper");
            Console.WriteLine(sb);
        }
    }

    public class Shuba : Salad
    {
        protected override void Layout()
        {
            Console.WriteLine("\tplace the salad in a deep bowl");
        }
        protected override void Mix()
        {
            StringBuilder sb = new();
            sb.AppendLine("\nMixing");
            sb.Append("  (layers)\n");
            sb.Append("\tpotatoes\n\tmayo\n\therring\n\tmayo\n\teggs\n\tcarrots\n\tmayo\n\tbeets\n\tmayo\n\tpotatoes\n\tbeets\n");
            Console.WriteLine(sb);
        }
        protected override void Cut()
        {
            //base.Cutting();
            StringBuilder sb = new();
            sb.AppendLine("\nCutting");
            sb.Append("\tChop off head, de-gut and de-bone herring\n\tchop herring into pieces\n\tpeel and shred the vegetables and eggs");
            Console.WriteLine(sb);
        }
        protected override void ThermalOperations()
        {
            StringBuilder sb = new();
            sb.AppendLine("\nBoiling/Baking/Frying");
            sb.Append("\tBoil vegetables and eggs\n\tboil beets 1-1.5 hours until tender\n");
            Console.WriteLine(sb);
        }
        protected override void PrepareIngredients()
        {
            StringBuilder sb = new();
            sb.AppendLine("list of Shuba ingredients:");
            sb.Append("\tfillets of herring\n\teggs\n\tpotatoes\n\tcarrots\n\tbeets\n\tonion\n\tmayonnaise\n\tparsley?\n");
            Console.WriteLine(sb);
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Shuba (1) or Caesar (2) ? ");
            var ch = Console.Read();
            Salad salad;
            if (ch == '1')
            {
                salad = new Shuba();
            }
            else
            {
                salad = new Caesar();
            }
            salad.Recipe();
        }
    }
}
