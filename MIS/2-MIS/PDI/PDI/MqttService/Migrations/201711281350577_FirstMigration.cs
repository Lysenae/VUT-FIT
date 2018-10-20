namespace MqttService.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class FirstMigration : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.MicrocontrollerEntities",
                c => new
                    {
                        Id = c.Guid(nullable: false, identity: true),
                        DeviceId = c.String(),
                        Powered = c.Boolean(nullable: false),
                        Temperature = c.Double(nullable: false),
                    })
                .PrimaryKey(t => t.Id);
            
            CreateTable(
                "dbo.PowerStripEntities",
                c => new
                    {
                        Id = c.Guid(nullable: false, identity: true),
                        DeviceId = c.String(),
                        Powered = c.Boolean(nullable: false),
                    })
                .PrimaryKey(t => t.Id);
            
        }
        
        public override void Down()
        {
            DropTable("dbo.PowerStripEntities");
            DropTable("dbo.MicrocontrollerEntities");
        }
    }
}
