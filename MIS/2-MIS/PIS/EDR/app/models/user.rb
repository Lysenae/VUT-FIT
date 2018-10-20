class User < ApplicationRecord
  has_secure_password
  belongs_to :employee
  belongs_to :right
  validates  :password, presence: true, length: { in: 8..20 }
  validates  :login, presence: true
  validates  :password_confirmation, presence: true, length: { in: 8..20 }
  validates  :login, uniqueness: true

  def material_coordinator?
    right.level == 3
  end

  def personel_clerk?
    right.level == 2
  end

  def technician?
    right.level == 1
  end

  def administrator?
    right.level == 0
  end

  # AccessGranted required field
  def is_admin
    administrator?
  end

  def self.all_active
    self.where(active: true)
  end
end
