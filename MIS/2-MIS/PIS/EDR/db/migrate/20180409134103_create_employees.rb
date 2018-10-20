class CreateEmployees < ActiveRecord::Migration[5.1]
  def change
    create_table :employees do |t|
      t.belongs_to :office, index: true
      t.string     :name,       null: false
      t.string     :surname,    null: false
      t.string     :email,      null: false
      t.date       :birth_date, null: false
      t.string     :phone_mobile
      t.string     :phone_office
      t.boolean    :active,  default: true
      t.timestamps
    end
  end
end
