@extends('layouts.app')

@section('content')

<div class="container">
    <div class="">
	<div class="panel panel-default">
	    <div class="panel-heading">
		Seznam zaměstnanců
	    </div>

	    <div class="panel-body">
	    <!-- Display Validation Errors -->
	    @include('common.errors')
		<div class="table-responsive">	
		<table class="table table-striped row-table">
		    <thead>
			<th>Jméno</th>
			<th>Příjmení</th>
			<th>Login</th>
			<th>Kancelář</th>
			<th>Pozice</th>
			<th>E-mail</th>
			<th>Mobil</th>
			<th>Telefon</th>
			<th></th>
		    </thead>
		    <tbody>
		    @foreach ($employees as $row)
		    <tr>
			<td class="table-text">
			    {{ $row->jmeno }}
			</td>
			<td class="table-text">
			    {{ $row->prijmeni }}
			</td>
			<td class="table-text">
			    {{ $row->login }}
			</td>
			<td class="table-text" style="text-align:center">
			    {{ $row->pracoviste }}
			</td>
			<td class="table-text">
			    {{ $row->popis }}
			</td>
			<td class="table-text">
			    {{ $row->mail }}
			</td>
			<td class="table-text">
			@if ($row->mobil > 0)
				{{ $row->mobil }}
			@endif			   
			</td>
			<td class="table-text">
			    @if ($row->telefon_kancelar > 0)
				{{ $row->telefon_kancelar }}
			    @endif			   
			</td>
			<td class="table-text">
			    <a href="/zamestnanec/{{ $row->zamID }}/show">upravit</a>
			</td>
		    </tr>
		    @endforeach
		    </tbody>
		</table>
		</div>
		<form action="/zamestnanci" method="POST" class="form-horizontal">
		    {{ csrf_field() }}

		    <!-- Add employee Button -->
		    <div class="form-group">
			<div class="col-sm-offset-0 col-sm-6">
			    <button type="submit" class="btn btn-default">
				<i class="fa fa-plus"></i> Přidat zaměstnance</button>
			</div>
		    </div>
		</form>
	    </div>
	</div>
</div>

    <!-- TODO: Current employees -->
@endsection
