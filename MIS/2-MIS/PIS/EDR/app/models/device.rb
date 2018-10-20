class Device < ApplicationRecord
  belongs_to :room
  belongs_to :employee
  validates  :brand, presence: true
  validates  :model, presence: true
  validates  :price, presence: true

  def being_transfered?
    Transfer.all_not_finished.each do |nf|
      if (nf.device == self)
        return true
      end  
    end
    return false
  end

  def self.all_active
    self.where(active: true)
  end

  def self.all_not_being_transfered
    self.all_active.select { |s| !s.being_transfered? }
  end

  def fullname
    "#{brand} #{model}"
  end
end
