class EmployeesController < ApplicationController
  before_action :authorize
  before_action :set_employee, only: [:show, :edit, :update, :destroy]

  EMPLOYEE_PROPERTIES = {
    name:         nil,
    surname:      nil,
    email:        nil,
    birth_date:   nil,
    phone_mobile: nil,
    phone_office: nil
  }

  # GET /employees
  # GET /employees.json
  def index
    @employees = Employee.all_active
  end

  # GET /employees/1
  # GET /employees/1.json
  def show
    access(:read)
  end

  # GET /employees/new
  def new
    access(:create)
    @employee = Employee.new
    @offices  = Office.all_active
  end

  # GET /employees/1/edit
  def edit
    access(:update)
    @employee = Employee.find(params[:id])
    @offices  = Office.all_active
  end

  # POST /employees
  # POST /employees.json
  def create
    access(:create)
    checked_params           = check_params(EMPLOYEE_PROPERTIES)
    checked_params["office"] = Office.find(employee_params[:office_id])
    @employee                = Employee.new(checked_params)
    respond_to do |format|
      if @employee.save
        format.html { redirect_to @employee}
        format.json { render :show, status: :created, location: @employee }
      else
        @offices = Office.all
        format.html { render :new }
        format.json { render json: @employee.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /employees/1
  # PATCH/PUT /employees/1.json
  def update
    access(:update)
    checked_params           = check_params(EMPLOYEE_PROPERTIES)
    checked_params["office"] = Office.find(employee_params[:office_id])
    respond_to do |format|
      if @employee.update(checked_params)
        format.html { redirect_to @employee}
        format.json { render :show, status: :ok, location: @employee }
      else
        @offices = Office.all
        format.html { render :edit }
        format.json { render json: @employee.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /employees/1
  # DELETE /employees/1.json
  def destroy
    access(:destroy)
    @employee.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to employees_url}
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_employee
      @employee = Employee.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def employee_params
      params.fetch(:employee, {})
    end

    def check_params(params)
      checked = {}
      params.each do |k, v|
        checked[k] = employee_params[k].blank? ? v : employee_params[k]
      end
      checked
    end

    def access(action)
      authorize!(action, Employee.new)
    end
end
