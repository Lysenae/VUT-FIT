class SessionsController < ApplicationController
  auto_session_timeout_actions

  def new
    redirect_to(root_url) unless session[:user_id].nil?
  end

  def create
    user = User.find_by_login(params[:login])
    if user && user.authenticate(params[:password])
      session[:user_id] = user.id
      redirect_to(root_url)
    else
      redirect_to(login_path, flash: { error: 'Login or password is invalid' })
    end
  end

  def destroy
    session[:user_id] = nil
    redirect_to(login_path)
  end

  def active
   render_session_status
  end
  
  def timeout
    render_session_timeout
  end
end
