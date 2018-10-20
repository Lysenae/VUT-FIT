using System;
using System.Collections.Generic;
using MqttService.Persistence;
using MqttService.Logging;
using MQTTnet;
using MQTTnet.Core.Client;
using System.Threading.Tasks;

namespace MqttService.Client
{
    public partial class Client
    {
        private string _brokerAddress;

        private IMqttClient   _Client { get; set; }
        public List<string>   SubscribedTopics { get; private set; }
        private SqlRepository _Repository { get; set; }

        public string BrokerAddress
        {
            get { return _brokerAddress; }
            set
            {
                if(value != _brokerAddress)
                {
                    _brokerAddress = value;
                }
            }
        }

        public bool IsConnected
        {
            get
            {
                if (this._Client == null)
                    return false;
                return this._Client.IsConnected;
            }
        }

        public Client(string brokerAddress)
        {
            this.SubscribedTopics = new List<string>();
            this._Repository      = new SqlRepository();
            this._Client          = new MqttFactory().CreateMqttClient();
            this._Client.ApplicationMessageReceived += OnMessageReceived;
            this.BrokerAddress = brokerAddress; // This also calls Connect()
        }

        public void ReinitializeSubscriptions()
        {
            if (this.SubscribedTopics.Count > 0)
            {
                Unsubscribe(this.SubscribedTopics.ToArray());
                this.SubscribedTopics.Clear();
            }
            Subscribe(Topic.McuAnnounce());
            Subscribe(Topic.StripAnnounce());
            SubscribeToSavedMcus();
            SubscribeToSavedPowerStrips();
        }

        public void Disconnect()
        {
            if (this.IsConnected)
            {
                this._Client.DisconnectAsync().Wait();
            }
        }

        public async Task Connect()
        {
            if(IsConnected)
                Disconnect();

            if(!(this.BrokerAddress.StartsWith("ws://") ||
            this.BrokerAddress.StartsWith("wss://")))
            {
                Logger.Error("Invalid protocol in '{0}'", this.BrokerAddress);
                return;
            }

            string useTls = null;
            var options = new MqttClientOptionsBuilder()
                .WithWebSocketServer(this.BrokerAddress)
                .WithClientId(Guid.NewGuid().ToString() + "-PDI-team01");
            if(this.BrokerAddress.StartsWith("wss://"))
            {
                options = options.WithTls();
                useTls = "with";
            }
            else
                useTls = "without";

            Logger.Info("Trying to connect to '{0}' {1} TLS", this.BrokerAddress, useTls);

            try
            {
                await this._Client.ConnectAsync(options.Build());
            }
            catch(Exception e)
            {
                Logger.Error("Failed to connect to {0} {1} TLS", this.BrokerAddress, useTls);
                Logger.Error(e.Message);
                Logger.Error(e.StackTrace);
            }

            ReinitializeSubscriptions();
            Logger.Info(this.IsConnected ? "Connected" : "Not connected");
        }
    }
}
