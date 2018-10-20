class WelcomeController < ApplicationController
  def index
    redirect_to(login_url) unless session[:user_id]
  end
end
