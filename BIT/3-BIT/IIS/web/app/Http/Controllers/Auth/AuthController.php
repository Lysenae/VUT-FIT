<?php

namespace App\Http\Controllers\Auth;

use Auth;
use App\User;
use Validator;
use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use Illuminate\Foundation\Auth\ThrottlesLogins;
use Illuminate\Foundation\Auth\AuthenticatesAndRegistersUsers;

class AuthController extends Controller
{
    /*
    |--------------------------------------------------------------------------
    | Registration & Login Controller
    |--------------------------------------------------------------------------
    |
    | This controller handles the registration of new users, as well as the
    | authentication of existing users. By default, this controller uses
    | a simple trait to add these behaviors. Why don't you explore it?
    |
    */

    use AuthenticatesAndRegistersUsers, ThrottlesLogins;
    protected $redirectPath = '/home';
    protected $loginPath = '/login';
    
    public function __construct()
    {
	$this->middleware('guest', ['except' => 'getLogout']);
    }
    
    protected function validator(array $data)
    {
	return Validator::make($data, [
	    'login' => 'required|max:255',
	    'password' => 'required|confirmed|min:6',
	]);
    }
    
    protected function create(array $data)
    {
	return User::create([
	    'login' => $data['login'],
	    'password' => bcrypt($data['password']),
	    'prava' => $data['prava'],
	]);
    }
    public function getLogout()
    {
	Auth::logout();
	return redirect('/login');

    }	     
    public function postLogin(Request $request)
    {
        $this->validate($request, [
            'login' => 'required', 'password' => 'required',
        ]);

        $throttles = $this->isUsingThrottlesLoginsTrait();

        if ($throttles && $this->hasTooManyLoginAttempts($request)) {
            return $this->sendLockoutResponse($request);
	}

	$login = $request->input('login');
	$pass = $request->input('password');
/*	if (Auth::attempt(['login' => $login, 'password' => $pass]))
	{
	    return redirect($this->redirectPath);
	}*/
	//$user = User::where('login', $login)->first();
	$user = array(
	    'login' => $login,
	    'password' =>  $pass);
	if (Auth::attempt($user))
	{
	    return redirect($this->redirectPath);
	}
/*	if ($user->password == md5($pass))
	{
	    if (Auth::login($user))
		return redirect($this->redirectPath);
	    else
		return redirect($this->loginPath())
		->withInput($request->only('login'))
		->withErrors([
		    'login' => "unable to make Auth::login" 
		]);
	}
	else
	    return redirect($this->loginPath())
	    ->withInput($request->only('login'))
	    ->withErrors([
		'login' => "passwords dont match" 
	    ]);
 */
	
    if ($throttles) {
            $this->incrementLoginAttempts($request);
        }
	//$msg = "heslo = ".$heslo[0]->heslo."<br> pass = $pass";
        return redirect($this->loginPath())
            ->withInput($request->only('login'))
            ->withErrors([
		'login' => $this->getFailedLoginMessage()
            ]);
    }
}
