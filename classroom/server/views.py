from django.shortcuts import render
from django.http import HttpResponse, HttpResponseNotFound

from server.models import HybridClassroom

# Create your views here.

def home(*args, **kwargs):
    return HttpResponse("6.810 Hybrid Classroom!")


'''Get most recent session's information'''
def session_info(*args, **kwargs):
    return HttpResponse(str(get_most_recent()))


'''Delete all sessions'''
def reset(*args, **kwargs):
    HybridClassroom.objects.all().delete()
    return HttpResponse("Deleted all sessions")


'''Creates a new session'''
def session(request, *args, **kwargs):
    new_session = HybridClassroom()
    new_session.save()
    return HttpResponse(f"Made a new session with id {new_session.session_id}")
    

'''
Return bear info in the form:
    "<hand_raised>; (<r>, <g>, <b>)"
'''
def bear(*args, **kwargs):
    # TODO: implement
    session = get_most_recent()
    color = str((session.color_red, session.color_green, session.color_blue))
    
    return HttpResponse(str(session.hand_raised) + "; " + color)


'''Activate high five if currently dectivated'''
def teacher_high_five(*args, **kwargs):
    session = get_most_recent()

    if not session.high_five:
        session.high_five = True
        session.save()
        return HttpResponse("High five is now activated")

    
    return HttpResponse("High five was already active")


'''Deactivate high five if currently activated'''
def student_high_five(*args, **kwargs):
    session = get_most_recent()

    if session.high_five:
        session.high_five = False
        session.save()
        return HttpResponse("High five is now deactivated")

    
    return HttpResponse("High five was already inactive")


# '''Display mousepad info; will probably delete'''
# def mousepad(*args, **kwargs):
#     session = get_most_recent()

#     pass


'''
Updates color of bear based on provided rgb parameters
    ?r=<red>&g=<green>&b=<blue>
        e.g. ?r=0&g=20&b=48
'''
def color(request, *args, **kwargs):
    session = get_most_recent()
    if session is None:
        return HttpResponse("No session created")
    
    red = request.GET.get('r')
    green = request.GET.get('g')
    blue = request.GET.get('b')

    if None in [red, green, blue]:
        return HttpResponse(f"Current color is {(session.color_red, session.color_green, session.color_blue)}")
    
    session.color_red = int(red)
    session.color_green = int(green)
    session.color_blue = int(blue)
    session.save()

    return HttpResponse(f"Set color to {(session.color_red, session.color_green, session.color_blue)}")


'''Toggles raised hand every time button is pushed'''
def raise_hand(*args, **kwargs):
    session = get_most_recent()
    session.hand_raised = False if session.hand_raised else True
    session.save()
    return HttpResponse(f"Toggled raised hand to {session.hand_raised}")


'''
Update `understanding` field based on `good` parameter
    ?good=true -> sets understanding to true
    ?good=false -> sets understanding to false
    ?good=<anything else> -> sets understanding back to None
'''
def understand(request, *args, **kwargs):
    session = get_most_recent()
    if session is None:
        return HttpResponse("No session created")
    
    understand_parameter = request.GET.get('good')
    if understand_parameter is None:
        return HttpResponse(f"Current understanding is {session.understanding}")

    if is_true(understand_parameter):
        session.understanding = True
        session.save()
        return HttpResponse("Understanding parameter set to True")

    elif is_false(understand_parameter):
        session.understanding = False
        session.save()
        return HttpResponse("Understanding parameter set to False")

    else:
        session.understanding = None
        session.save()
        return HttpResponse("Understanding parameter set to None")


'''
Activate applause based on request parameter `active`
    ?active=true -> sets applause to true
    ?active=false -> sets applause to false
'''
def applause(request, *args, **kwargs):
    session = get_most_recent()
    if session is None:
        return HttpResponse("No session created")
    
    applause_parameter = request.GET.get('active')

    if is_true(applause_parameter):
        session.applause = True
        session.save()
        return HttpResponse("Applause parameter set to True")

    elif is_false(applause_parameter):
        session.applause = False
        session.save()
        return HttpResponse("Applause parameter set to False")

    else:
        return HttpResponse("Did not understand applause parameter")


'''Helper function to get most recent session'''
def get_most_recent():
    if HybridClassroom.objects.count() > 0:
        return HybridClassroom.objects.all().order_by('-session_id')[0]

    return None


'''Helper function to check if a string is true'''
def is_true(string):
    return string in ['True', 'true', '1', "TRUE", "yes", "Y", "YES"]


'''Helper function to check if a string is false'''
def is_false(string):
    return string in ['False', 'false', '0', "FALSE", "no", "N", "NO"]

