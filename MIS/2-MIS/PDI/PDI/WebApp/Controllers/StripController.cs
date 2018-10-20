using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using MqttService.Models;
using System.Threading.Tasks;

namespace WebApp.Controllers
{
    public class StripController : ApiController
    {
        private static MqttService.MqttService srv = new MqttService.MqttService();

        private async Task<MqttService.MqttService> GetMqttService()
        {
            if (!srv.GetClientConnected())
                await srv.ConnectClient();

            return srv;
        }

        [HttpGet]
        [ActionName("Read")]
        public async Task<List<PowerStrip>> GetAllStrips()
        {
            MqttService.MqttService lService = await GetMqttService();
            List<PowerStrip> result = lService.GetPowerStrips().ToList();
            return result;
        }

        [HttpGet]
        [ActionName("Read")]
        public async Task<IHttpActionResult> GetStrip(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            var strips = lService.GetPowerStrips().ToList();
            var strip = strips.FirstOrDefault((p) => p.DeviceId == id);
            if (strip == null)
            {
                return NotFound();
            }
            return Ok(strip);
        }

        [HttpPost]
        [ActionName("Poweroff")]
        public async Task<IHttpActionResult> PostPowerOff(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            lService.CommandStripPower(id, 0);
            return Ok(200);
        }

        [HttpPost]
        [ActionName("Poweron")]
        public async Task<IHttpActionResult> PostPowerOn(string id)
        {
            MqttService.MqttService lService = await GetMqttService();
            lService.CommandStripPower(id, 1);
            return Ok(200);
        }
    }
}
