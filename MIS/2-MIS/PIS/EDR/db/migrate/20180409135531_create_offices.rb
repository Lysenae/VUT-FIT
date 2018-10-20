class CreateOffices < ActiveRecord::Migration[5.1]
  def change
    create_table :offices do |t|
      t.belongs_to :room, index: true
      t.integer    :ethernet_sockets,   default: 0
      t.integer    :electrical_sockets, default: 0
      t.boolean    :active, default: true
      t.timestamps
    end
  end
end
