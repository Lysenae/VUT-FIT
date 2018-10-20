@extends('layouts.app')

@section('content')

<div class="container">
    <div class="row">
    <div class="col-sm-4">
	<div class="panel panel-default">
	    <div class="panel-heading">
	    <b>	    
		Informace o přihlášeném uživateli:
	    </b>
	    </div>
	    <div class="panel-body">
		<table class="table table-striped row-table">
		    <thead>
		    </thead>
		    <tbody>
		    <tr>
			<td>
			Jméno:			
			</td>
			<td>
			{{ $rec->jmeno }}
			</td>
		    </tr>
		    <tr>
			<td>
			Příjmení:
			</td>
			<td>
			    {{ $rec->prijmeni }}
			</td>
		    </tr>
		    <tr>
			<td>
			Login:
			</td>
			<td>
			{{ $rec->login }}
			</td>
		    </tr>
		    <tr>
			<td>
			Pozice:
			</td>
			<td>
			{{ $rec->popis }}
			</td>
		    </tr>
			@if ($rec->mail != "")
		    <tr>
			<td>
			Email:
			</td>
			<td>
			{{ $rec->mail }}
			</td>
		    </tr>
			@endif			   
			@if ($rec->mobil > 0)
		    <tr>
			<td>
			Mobil:
			</td>
			<td>
				{{ $row->mobil }}
			</td>
		    </tr>
			@endif			   
			@if ($rec->telefon_kancelar > 0)
		    <tr>
			<td>
			Telefon:
			</td>
			<td>
				{{ $rec->telefon_kancelar }}
			</td>
		    </tr>
			@endif			   
		    </tbody>
		</table>
	    </div>
	</div>
    </div>
    <div class="col-sm-8">
    	<div class="panel panel-default">
	@if ($neprovedene)
	    <div class="panel-heading">
	    <b>	    
		Mé neschválené přesuny:
		
	    </b>
	    </div>
	    <div>
		<table class="table table-striped row-table">
		    <thead>
			<th>Zařízení</th>
			<th>Kam</th>
		    </thead>
		    <tbody>
		    @foreach ($neprovedene as $row)
		    <tr class="danger">
			<td class="table-text">
			    #{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
			</td>
			<td class="table-text">
			    {{ $row->kam }}
			</td>
    
		    </tr>
		    @endforeach

		    </tbody>
		</table>
	    </div>
	@else
	    <div class="panel-heading">
		Nemáte žádná přesunutí čekající na schválení.
	    </div>
	@endif
	</div>
	<div class="panel panel-default">
	@if ($neopravene)
	    <div class="panel-heading">
	    <b>	    
		Mé nahlášené neopravené vady:
	    </b>
	    </div>
		<table class="table table-striped row-table">
		    <thead>
			<th>Zařízení</th>
			<th>Popis vady</th>
		    </thead>
		    <tbody>
		    @foreach ($neopravene as $row)
		    <tr class="danger">
			<td class="table-text">
			    #{{ $row->zarID }} {{ $row->znacka }} {{ $row->model }}
			</td>
			<td class="table-text">
			    {{ $row->popis }}
			</td>
    
		    </tr>
		    @endforeach

		    </tbody>
		</table>
	@else
	    <div class="panel-heading">
		Nemáte žádné poškozené zařízení čekající na opravení.
	    </div>
	@endif
	</div>
	</div>
	</div>
    </div>
</div>
</div>

    <!-- TODO: Current employees -->
@endsection
