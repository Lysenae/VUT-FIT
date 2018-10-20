class Right < ApplicationRecord
  has_many  :users, dependent: :destroy
  validates :name,  presence:  true
  validates :level, presence:  true

  def self.admin
    self.find_by_level(0)
  end

  def self.technician
    self.find_by_level(1)
  end

  def self.personel_clerk
    self.find_by_level(2)
  end

  def self.material_coordinator
    self.find_by_level(3)
  end

  def self.employee
    self.find_by_level(4)
  end
end
