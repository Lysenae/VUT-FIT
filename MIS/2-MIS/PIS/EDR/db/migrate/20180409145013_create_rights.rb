class CreateRights < ActiveRecord::Migration[5.1]
  def change
    create_table :rights do |t|
      t.string  :name,  null: false
      t.integer :level, default: -1
      t.string  :description
      t.timestamps
    end
  end
end
