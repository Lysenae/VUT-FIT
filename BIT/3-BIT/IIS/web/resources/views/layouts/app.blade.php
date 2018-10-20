<!DOCTYPE html>
<html lang="cs">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="csrf-token" content="{{ csrf_token() }}">
    <title>Správa CVT zařízení</title>

    <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css" rel="stylesheet" type="text/css">

    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.0.0-alpha1/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>

    <style>
	body {
	    font-family: 'sans-serif';
	    margin-top: 25px;
	    background-color: #f5f5f5;
	}
	button .fa {
	    margin-right: 6px;
	}
	.table-text div {
	    padding-top: 6px;
	}
	.form-control:focus {
  z-index: 2;
}
    </style>
</head>

<body>
    <div class="container">
	<nav class="navbar navbar-default">
	    <div class="container-fluid">
		<div class="navbar-header">
		<button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false">
		    <span class="sr-only">Toggle navigation</span>
		    <span class="icon-bar"></span>
		    <span class="icon-bar"></span>
		    <span class="icon-bar"></span>
		</button>
		<a class={{ $active=='home' ? 'navbar-brand active' : 'navbar-brand' }} href="/home">Domů</a>	
		</div>
		<div id="navbar" class="navbar-collapse collapse">

		<ul class="nav navbar-nav">
		@if (in_array(Auth::user()->prava,[5,3]))
		   <li class={{ $active=='zamestnanci' ? 'active' : '' }}> 
			<a href="/zamestnanci">Zaměstnanci</a></li>
		@endif
		@if (in_array(Auth::user()->prava,[5,3]))
		   <li class={{ $active=='oddeleni' ? 'active' : '' }}> 
			<a href="/oddeleni">Oddělení</a></li>
		@endif
		@if (in_array(Auth::user()->prava,[5,2]))
		   <li class={{ $active=='mistnosti' ? 'active' : '' }}> 
			 <a href="/mistnosti">Místnosti</a></li>
		@endif
		@if (in_array(Auth::user()->prava,[5,2]))
		   <li class={{ $active=='zarizeni' ? 'active' : '' }}> 
			 <a href="/zarizeni">Zařízení</a></li>
		@endif
		@if (in_array(Auth::user()->prava,[5,4]))
		   <li class={{ $active=='poruchy' ? 'active' : '' }}> 
			 <a href="/poruchy">Poruchy</a></li>
		@endif
		@if (in_array(Auth::user()->prava,[5,2]))
		   <li class={{ $active=='presuny' ? 'active' : '' }}> 
			 <a href="/presuny">Přesuny</a></li>
		@endif
		</ul>
		<ul class="nav navbar-nav navbar-right"  > 
	    	<li class="dropdown">
	           <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">Mé akce<span class="caret"></span></a>
		<ul class="dropdown-menu">
		    <li><a href="/porucha">Nahlásit poruchu</a></li>
		    <li><a href="/presun">Zažádat o přesun</a></li>
		    <li role="separator" class="divider"></li>
		    <li><a href="/logout">Odhlásit</a></li>
		</ul>
		</ul>
		</div>		
	    </div>
	</nav>
    </div>

    @yield('content')
</body>
</html>
