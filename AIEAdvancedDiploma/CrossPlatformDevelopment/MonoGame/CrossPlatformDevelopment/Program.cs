////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Program.cs
////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;

namespace CrossPlatformDevelopment
{
#if WINDOWS || LINUX
    public static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            using (var application = new Application())
            {
                application.Run();
            }
        }
    }
#endif
}
