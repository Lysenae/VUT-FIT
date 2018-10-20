<?php

namespace App;

use Illuminate\Database\Eloquent\Model;
use Validator;
use DB;
use Auth;
class Poruchy  extends Model
{
    /**
     * Get a validator for an incoming registration request.
     *
     * @param  array  $data
     * @return \Illuminate\Contracts\Validation\Validator
     */
    protected function validator(array $data)
    {
	$rules = [
		'k_oprave' => 'required|max:5',
		'popis' => 'max:255',
	    ];
	return Validator::make($data, $rules);
    }

    /**
     * Create a new user instance after a valid registration.
     *
     * @param  array  $data
     * @return User
     */
    public static function createPor(array $data)
    {
        DB::table('iis_porucha')->insert([
            'k_oprave' => $data['k_oprave'],
            'oznamujici' => Auth::user()->id,
            'popis' => $data['popis'],
        ]);
    }


    public static function activate(array $data)
    {
	if (isset($data['act']))
	{
		foreach($data['act'] as $porucha)
		{
		    DB::table('iis_porucha')
			->where('poruchaID', $porucha)
			->update([
			'technik' => Auth::user()->id, 
		    ]);
		}
	}
	else 
	   return redirect()->back()->withError(['submit' => 'Musite vybrat nektery zaznam']);
    }
}
