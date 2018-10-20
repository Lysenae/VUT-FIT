class Department < ApplicationRecord
  has_many  :rooms,    dependent: :destroy
  validates :name,     presence: true
  validates :shortcut, presence: true
  validates :shortcut, uniqueness: true

  def self.all_active
    self.where(active: true)
  end
end
