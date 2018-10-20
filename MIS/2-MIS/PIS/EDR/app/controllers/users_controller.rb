class UsersController < ApplicationController
  before_action :authorize
  before_action :set_user, only: [:show, :edit, :update, :destroy]

  # GET /users
  # GET /users.json
  def index
    @users = User.all_active
  end

  # GET /users/1
  # GET /users/1.json
  def show
    authorize! :read, @user
  end

  # GET /users/new
  def new
    @user = User.new
    authorize! :create, @user
    init_selects(@user)
  end

  # GET /users/1/edit
  def edit
    @user = User.find(params[:id])
    authorize! :update, @user
    init_selects(@user)
  end

  # POST /users
  # POST /users.json
  def create
    authorize! :create, @user
    checked_params = Hash.new
    checked_params["employee"] = Employee.find(user_params["employee"])
    checked_params["right"]    = Right.find(user_params["right"])
    checked_params["login"]    = user_params["login"]
    checked_params["password"] = user_params["password"]
    checked_params["password_confirmation"] =
      user_params["password_confirmation"]

    @user = User.new(checked_params)
    init_selects(@user)

    respond_to do |format|
      if @user.save
        format.html { redirect_to(@user) }
        format.json { render(:show, status: :created, location: @user) }
      else
        format.html { render(:new) }
        format.json { render(json: @user.errors, status: :unprocessable_entity) }
      end
    end
  end

  # PATCH/PUT /users/1
  # PATCH/PUT /users/1.json
  def update
    authorize! :update, @user
    checked_params = Hash.new
    checked_params["employee"] = Employee.find(user_params["employee"])
    checked_params["right"]    = Right.find(user_params["right"])
    checked_params["login"]    = user_params["login"]
    checked_params["password"] = user_params["password"]

    init_selects(@user)

    respond_to do |format|
      if @user.update(checked_params)
        format.html { redirect_to(@user) }
        format.json { render(:show, status: :ok, location: @user) }
      else
        format.html { render(:edit) }
        format.json { render(json: @user.errors, status: :unprocessable_entity) }
      end
    end
  end

  # DELETE /users/1
  # DELETE /users/1.json
  def destroy
    authorize! :destroy, @user
    @user.update_attribute(:active, false)
    respond_to do |format|
      format.html { redirect_to(users_url) }
      format.json { head(:no_content) }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_user
      @user = User.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def user_params
      params.require(:user).permit(
        :login, :password, :password_confirmation, :employee, :right
      )
    end

    def init_selects(user)
      @employees        = Employee.all_without_user_account
      @rights           = Right.all
      @current_employee = user && user.employee ? user.employee.id : nil
      @current_right    = user && user.right ? user.right.id : nil
    end
end
