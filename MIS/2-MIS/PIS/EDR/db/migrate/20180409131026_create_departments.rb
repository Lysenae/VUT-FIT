class CreateDepartments < ActiveRecord::Migration[5.1]
  def change
    create_table :departments do |t|
      t.string  :name, null: false
      t.string  :shortcut, null: false
      t.text    :description
      t.boolean :active, default: true
      t.timestamps
    end
  end
end
