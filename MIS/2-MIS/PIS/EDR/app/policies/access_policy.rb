class AccessPolicy
  include AccessGranted::Policy

  def configure
    role :admin, { is_admin: true } do
      can :manage, Employee
      can :manage, Device
      can :manage, Schoolroom
      can :manage, Office
      can :manage, Department
      can :manage, Failure
      can :manage, Transfer
      can :manage, User
      can :index,  User
    end

    role :technician, proc { |u| u.technician? } do
      can :manage, Failure
    end

    role :material_coordinator, proc { |u| u.material_coordinator? } do
      can :manage, Device
      can :manage, Transfer
    end

    role :personel_clerk, proc { |u| u.personel_clerk? } do
      can :manage, Employee
    end

    role :enployee do
      can :read, Employee
      can :read, Device
      can :read, Schoolroom
      can :read, Office
      can :read, Department
      can :read, Failure
      can :create, Failure
      can :read, Transfer
      can :create, Transfer
    end
  end
end
