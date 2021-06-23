using System;
using System.IO;
using System.IO.Pipes;
using System.Drawing;

class PipeServer
{
    public static byte[] ImageToByteArray(System.Drawing.Image imageIn)
    {
        using (var ms = new MemoryStream())
        {
            imageIn.Save(ms, imageIn.RawFormat);
            return ms.ToArray();
        }
    }

    static void Main()
    {
        while (true)
        {
            using (NamedPipeServerStream pipeServer =
            new NamedPipeServerStream(@"Pipe", PipeDirection.InOut, 20, PipeTransmissionMode.Byte))
            {

                Console.WriteLine("NamedPipeServerStream object created.");

                // Wait for a client to connect
                pipeServer.ReadMode = PipeTransmissionMode.Byte;
                Console.Write("Waiting for client connection...");
                pipeServer.WaitForConnection();
                Console.WriteLine("Client connected.");
                StreamWriter sw;
                try
                {
                    using (StreamReader sr = new StreamReader(pipeServer))
                    {
                        Console.WriteLine(sr.ReadLine());
                        //файл изображения должен быть создан, затем он считывается и передаётся клиенту
                        var myf = File.ReadAllBytes("C:\\img\\image.png");
                        pipeServer.Write(myf,0,myf.Length);
                    }

                    // Read user input and send that to the client process.
                }
                // Catch the IOException that is raised if the 9pipe is broken
                // or disconnected.
                catch (IOException e)
                {
                    Console.WriteLine("ERROR: {0}", e.Message);
                }

            }
        }
    }
}
