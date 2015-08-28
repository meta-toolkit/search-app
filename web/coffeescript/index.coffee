$ ->
  window.location.hash = "#bm25"
  $(".input-group").keypress (k) ->
    if k.which == 13 # enter key pressed
      $("#search_button").click()
      return false;
  $("#search_button").click ->
    query = $("#query_text").val()
    if query.length != 0
      $("#search_results_list").empty()
      $.ajax "/search-api",
        type: "POST"
        dataType: "json"
        data: JSON.stringify
          jsonrpc: "2.0"
          method: "search"
          params:
            query_text: query
            ranker_method: window.location.hash.slice(1)
          id: 1
        success: (data, stat, xhr) ->
          print_results(data.result)
        failure: (axhr, stat, err) ->
          $("#search_results_list").append("<li>Something bad happened!</li>")

print_results = (result) ->
  for doc in result.results
    html = "<li><h4><a href='#'>#{doc.path}</a>"
    html += "<small class='pull-right'>#{doc.score.toFixed(4)}</small></h4></li>"
    $("#search_results_list").append(html)
