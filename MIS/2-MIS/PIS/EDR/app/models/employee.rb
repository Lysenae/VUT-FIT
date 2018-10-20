class Employee < ApplicationRecord
  validate :email_must_be_unique
  belongs_to :office
  validates  :name,       presence: true
  validates  :surname,    presence: true
  validates  :birth_date, presence: true
  validates  :email,      presence: true
  validates  :email, uniqueness: true
  before_destroy :remove_user

  def self.all_active
    self.where(active: true)
  end

  def remove_user
    User.where(employee_id: id) do |e|
      e.destroy
    end
  end

  def user
    User.where(employee_id: id).first
  end

  def user?
    !user.nil?
  end

  def fullname
    "#{name} #{surname} (#{birth_date.strftime('%F')})"
  end

  def fullname_with_bday
    "#{fullname} (#{birth_date.strftime('%F')})"
  end

  def email_must_be_unique
    Employee.all.each do |s|
      if (s.email == email) and (s != self)
        errors.add(:email, "email must be unique")
      end
    end
  end

  def self.all_technicians
    technicians = Array.new
    User.all.each do |u|
      if (u.right.level == 1)
        technicians.push(u.employee)
      end
    end
    return technicians
  end

  def self.all_without_user_account
    emp_with_acc = Array.new
    Employee.all.each do |e|
      User.all.each do |u|
        if u.employee == e
          emp_with_acc.push(e)  
        end
      end
    end
    Employee.all - emp_with_acc
  end
end
