class DepartmentsController < ApplicationController
  before_action :authorize
  before_action :set_department, only: [:show, :edit, :update, :destroy]

  DEPARTMENT_PROPERTIES = {
    name:        nil,
    shortcut:    nil,
    description: nil
  }

  # GET /departments
  # GET /departments.json
  def index
    @departments = Department.all_active
  end

  # GET /departments/1
  # GET /departments/1.json
  def show
    redirect_to(departments_path)
  end

  # GET /departments/new
  def new
    @department = Department.new
    access(:create)

  end

  # GET /departments/1/edit
  def edit
    @department = Department.find(params[:id])
    access(:update)
  end

  # POST /departments
  # POST /departments.json
  def create
    access(:create)
    checked_params = check_params(DEPARTMENT_PROPERTIES)
    @department = Department.new(checked_params)
    respond_to do |format|
      if @department.save
        format.html { redirect_to departments_path}
        format.json { render :show, status: :created, location: departments_path }
      else
        format.html { render :new }
        format.json { render json: @department.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /departments/1
  # PATCH/PUT /departments/1.json
  def update
    access(:update)
    checked_params = check_params(DEPARTMENT_PROPERTIES)
    respond_to do |format|
      if @department.update(checked_params)
        format.html { redirect_to departments_path}
        format.json { render :show, status: :ok, location: departments_path }
      else
        format.html { render :edit }
        format.json { render json: @department.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /departments/1
  # DELETE /departments/1.json
  def destroy
    access(:destroy)
    @department.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to departments_url}
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_department
      @department = Department.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def department_params
      params.fetch(:department, {})
    end

    def check_params(params)
      checked = {}
      params.each do |k, v|
        checked[k] = department_params[k].blank? ? v : department_params[k]
      end
      checked
    end

    def access(action)
      authorize!(action, Department.new)
    end
end
