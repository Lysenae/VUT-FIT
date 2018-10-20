using System;
using System.Linq;
using System.Collections.Generic;

using MqttService.Models;
using MqttService.Persistence;
using MqttService.Client;

namespace ConsoleTest
{
    class Program
    {
        static void Main(string[] args)
        {
            /*var svc = new MqttService.MqttService();
            var mc = new Microcontroller("aaaa", false, 64.2);
            var ps = new PowerStrip("bbbb");

            Console.WriteLine("MC: {0}, {1}, {2}", mc.DeviceId, mc.Powered, mc.Temperature);
            Console.WriteLine("PS: {0}, {1}", ps.DeviceId, ps.Powered);

            var repo = new SqlRepository();

            Console.WriteLine("Trying to load data");
            List<Microcontroller> mcrs = repo.Microcontrollers.All().ToList();
            List<PowerStrip> pwss = repo.PowerStrips.All().ToList();

            Console.WriteLine("Microcontrollers: {0}", mcrs.Count);
            Console.WriteLine("Power Strips: {0}", pwss.Count);

            if (mcrs.Count == 0)
                repo.Microcontrollers.Add(mc);
            if (pwss.Count == 0)
                repo.PowerStrips.Add(ps);*/
            /*
                  Console.WriteLine("Connecting");
                  var c = new Client("ws://mqtt-xklima22.azurewebsites.net:80/mqtt");
                  Console.WriteLine("Connected");
            */

            MqttService.MqttService srv = new MqttService.MqttService();
            srv.ConnectClient().Wait();
            Console.WriteLine("Connected");
            List<Microcontroller> mcus = srv.GetMicrocontrollers().ToList();
            Console.ReadKey();
        }
    }
}
