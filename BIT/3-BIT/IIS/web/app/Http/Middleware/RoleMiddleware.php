<?php

namespace App\Http\Middleware;

use Closure;

class RoleMiddleware
{
    /**
     *      * Run the request filter.
     *           *
     *                * @param  \Illuminate\Http\Request  $request
     *                     * @param  \Closure  $next
     *                          * @param  string  $role
     *                               * @return mixed
     *                                    */
    public function handle($request, Closure $next)
    {
	$actions = $request->route()->getAction();
	$roles = $actions['roles'];
	if (!in_array($request->user()->prava, $roles)) {
                return response('Na tuto operaci nemáte oprávnění. <a href="/home">Zpět do systému</a>', 401);
	}

	return $next($request);
    }
}
?>
