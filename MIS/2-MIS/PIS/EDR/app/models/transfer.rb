class Transfer < ApplicationRecord
  validate :from_and_to_must_be_different
  belongs_to :device
  belongs_to :author,       class_name: :Employee, foreign_key: :author_id
  belongs_to :approved,     class_name: :Employee, foreign_key: :approved_id, optional: true
  belongs_to :from,         class_name: :Room,     foreign_key: :from_id
  belongs_to :to,           class_name: :Room,     foreign_key: :to_id
  validates  :requested_at, presence: true
  validates  :reason,       presence: true

  def from_and_to_must_be_different
    if (from == to)
      errors.add(:to, "to cannot equal from")
    end
  end

  def finished?
    if (done_at != nil) or (active == false)
      true
    else
      false
    end
  end

  def self.all_not_finished
    self.all.select{|s| !s.finished?}  
  end

  def self.all_active
    self.where(active: true)
  end

end
