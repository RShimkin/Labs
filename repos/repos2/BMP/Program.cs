using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

namespace BMP
{
    class Program
    {
        public static Bitmap ToGrayscale(Bitmap bmp)
        {
            var result = new Bitmap(bmp.Width, bmp.Height, PixelFormat.Format8bppIndexed);

            BitmapData data = result.LockBits(new Rectangle(0, 0, result.Width, result.Height), ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);

            // Copy the bytes from the image into a byte array
            byte[] bytes = new byte[data.Height * data.Stride];
            Marshal.Copy(data.Scan0, bytes, 0, bytes.Length);

            for (int y = 0; y < bmp.Height; y++)
            {
                for (int x = 0; x < bmp.Width; x++)
                {
                    var c = bmp.GetPixel(x, y);
                    var rgb = (byte)((c.R + c.G + c.B)/3);

                    bytes[x * data.Height + y] = rgb;
                }
            }

            // Copy the bytes from the byte array into the image
            Marshal.Copy(bytes, 0, data.Scan0, bytes.Length);

            result.UnlockBits(data);

            return result;
        }

        public static void BlackWhite(Bitmap bm)
        {
            var newbm = bm.Clone(new Rectangle(0, 0, bm.Width, bm.Height), PixelFormat.Format1bppIndexed);
            newbm.Save(@"D:\BMP\BlackWhite.bmp");
        }
        static void Main(string[] args)
        {
            string inname, outname;
            if (args.Length < 2)
            {
                inname = Console.ReadLine();
                outname = Console.ReadLine();
            }
            else
            {
                inname = args[0];
                outname = args[1];
            }
            //D:\BMP\a.bmp
            //D:\BMP\edited.bmp
            var bitmap = new Bitmap(inname);
            
            Console.WriteLine(bitmap.Width);

            //int num = bitmap.Height * bitmap.Width;
            //int sum = 0;
            Color color;
            int rgb;
            int top = Console.CursorTop;
            int left = Console.CursorLeft;

            Console.WriteLine(Path.GetExtension(inname));
            Console.WriteLine(bitmap.PixelFormat);
            //ToGrayscale(bitmap).Save(@"D:\BMP\8bit2.bmp");
            BlackWhite(bitmap);
            Console.ReadKey();

            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    color = bitmap.GetPixel(x, y);
                    rgb = (int)Math.Round(color.R * .299 + color.G * .587 + color.B * .114);
                    //rgb = (int)Math.Round(color.R * .0 + color.G * .5 + color.B * 0);
                    bitmap.SetPixel(x, y, Color.FromArgb(rgb, rgb, rgb));
                }
                Console.SetCursorPosition(left, top);
                Console.WriteLine($"{(float)y / bitmap.Height * 100 :F0}%");
            }

            bitmap.Save(outname);
            //bitmap.Dispose();
            Bitmap small = new Bitmap(@"D:\BMP\8bit2.bmp");
            Console.WriteLine(small.PixelFormat);
        }
    }
}
