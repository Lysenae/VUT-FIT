class CreateUsers < ActiveRecord::Migration[5.1]
  def change
    create_table :users do |t|
      t.belongs_to :employee,        index: true
      t.belongs_to :right,           index: true
      t.string     :login,           null: false
      t.string     :password_digest, null: false
      t.boolean    :active, default: true
      t.timestamps
    end
  end
end
