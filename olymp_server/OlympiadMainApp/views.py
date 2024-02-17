import os
import subprocess
from bson import ObjectId
import datetime as dt
from django.utils import timezone
from django.http import JsonResponse
from django.shortcuts import redirect, render
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import csrf_exempt
from django.core.files.base import ContentFile
from django.core.files.storage import default_storage
from django.db.models import Q
from django.contrib import messages

from .forms import *
from .models import *
from .workers import *
from Config import config

from OlympiadUsers.access import *

def servertime(request):
    now1 = dt.datetime.now()
    now2 = timezone.now()
    return JsonResponse({'servertime1': now1, 'servertime2': now2}, status=200)

def tasks(request):
    #tasks = StandaloneTask.objects.all()
    #tasks = StandaloneTask.objects.exclude(Q(creator=request.user) & Q(hidden=True))
    if request.user.is_authenticated:
        tasks = StandaloneTask.objects.exclude(creator=request.user)
        own_tasks = StandaloneTask.objects.filter(creator=request.user)
    else:
        tasks = StandaloneTask.objects.all()
        own_tasks = []
    tasks_arr = [StandaloneTaskViewModel(task) for task in tasks]
    own_arr = [StandaloneTaskViewModel(task) for task in own_tasks]
    own_exist = len(own_arr) > 0
    allow_create = has_groups(request.user, 'Creators')
    #print(request.user.groups)
    return render(request, 'OlympiadMainApp/standalonetasks.html',
                   {'title': 'Список отдельных заданий:', 'allow_create': allow_create,
                    'tasks': tasks_arr, 'own_tasks': own_arr, 'own_exist': own_exist })

def olympiads(request):
    olympiads = Olympiad.objects.all()
    arr = [OlympiadViewModel(olymp, request.user) for olymp in olympiads]
    allow_create = has_groups(request.user, 'Creators')
    return render(request, 'OlympiadMainApp/olympiads.html', 
                  {'title': 'Список олимпиад','olympiads': arr, 'allow_create': allow_create})

def update_task(request, task_name):
    curtask = Task.objects.get(name=task_name)
    if request.method == 'POST':
        form = TestDataForm(request.POST)
        if form.is_valid():
            formcontent = form.cleaned_data
            input =  formcontent['input']
            output = formcontent['output']
            # td = TestData(testinput=input, testoutput=output)
            if isinstance(curtask.tests, list):
                curtask.tests.append({
                    'testinput': input,
                    'testoutput': output
                })
            else:
                curtask.tests = [{
                    'testinput': input,
                    'testoutput': output
                }]
            curtask.save()
        else:
            print('invalid testdata form')
    else:
        form = TestDataForm()
    
    return render(request, 'OlympiadMainApp/updatetask.html', {'task': curtask, 'form': form })'''

def code(request):
    if request.method == 'POST':
        form = MyCodeForm(request.POST)
        if form.is_valid():
            formcontent = form.cleaned_data
            code = formcontent['content']
            lang = formcontent['plang']
            uname = None
            if request.user.is_authenticated:
                uname = request.user.username
            (compil_status, compil_res, prog_name, folder_name) = prepare_program(code, lang, uname)
            if compil_status == prog_statuses.COMPILED_ERR:
                print('compilation error!')
                remove_program(folder_name, None, prog_name)
                return JsonResponse({ 'text': 'compilation error!', 'error': True }, status=200)
            (run_status, run_res) = run_program(prog_name, folder_name, lang)
            remove_program(folder_name, None, prog_name)
            if run_status == prog_statuses.RUN_SUC:
                out = run_res.stdout.decode()
                print(out)
                return JsonResponse({ 'text': out, 'error': False }, status=200)
            elif run_status == prog_statuses.RUN_TOE:
                return JsonResponse({ 'text': 'timeout violation', 'error': True }, status=200)
            else:
                print('runtime error!')
                return JsonResponse({ 'text': 'runtime error!', 'error': True }, status=200)
        else:
            print('invalid form')
    else:
        form = MyCodeForm()

    return render(request, 'OlympiadMainApp/code.html', 
           {'title': 'Удалённый запуск кода', 'form': form })

@group_required_or_anon('Participants')
def stask(request, task_oid):
    curtask = StandaloneTask.objects.get(_id=ObjectId(task_oid))
    now = dt.datetime.now().astimezone(curtask.since.tzinfo)
    form = CodeForm(request.POST or None)
    #print(request.META.get("REMOTE_ADDR"))
    if form.is_valid():
        if request.user.is_authenticated:
            tr = curtask.results.filter(user=request.user).first()
        else:
            tr = curtask.results.filter(username=str(request.META.get("REMOTE_ADDR"))).first()
        print(tr)
        if not tr:
            print("StandaloneTaskResult not found")
            return JsonResponse({'text': 'Ошибка сессии', 'error': True}, status=200)
        tr.inner_attempts += 1
        tr.save()
        formcontent = form.cleaned_data
        code, lang = formcontent['content'], formcontent['plang']
        uname = request.user.username if request.user.is_authenticated else None

        (compil_status, compil_res, prog_name, folder_name) = prepare_program(code, lang, uname)
        if compil_status == prog_statuses.COMPILED_ERR:
            return JsonResponse({ 'text': 'Ошибка компиляции!', 'error': True }, status=200)

        run_status, match = prog_statuses.RUN_SUC, True
        for test in curtask.tests:
            inp, out = test['testinput'], test['testoutput']
            input_file_name = f"{folder_name}/input.txt"
            output_file_name = f"{folder_name}/output.txt"
            file = open(input_file_name, 'w')
            file.write(inp)
            file.close()
            (run_status, run_res) = run_program(prog_name, folder_name, lang)
            print(run_res, run_status)
            if run_status != prog_statuses.RUN_SUC:
                break
            res = run_res.stdout.decode('utf-8')
            if os.path.isfile(output_file_name):
                file = open(output_file_name, "r")
                res = file.read()
                file.close()
            print(res.strip(), "!", out.strip())
            if res.strip() != out.strip():
                print("!",res.strip())
                match = False
                break

        remove_program(folder_name, input_file_name, None)
        remove_program(folder_name, output_file_name, None)
        remove_program(folder_name, None, prog_name)

        if not match:
            return JsonResponse({ 'text': 'Неверный результат работы программы', 'error': True }, status=200)
        if run_status == prog_statuses.RUN_ERR:
            return JsonResponse({ 'text': 'Ошибка времени выполнения!', 'error': True }, status=200)
        if run_status == prog_statuses.RUN_TOE:
            return JsonResponse({ 'text': 'Слишком долгое выполнения!', 'error': True }, status=200)

        tr.finished = now
        tr.points = curtask.points
        tr.success = True
        curtask.results_number += 1
        tr.save()
        curtask.save()
        return JsonResponse({ 'text': 'Тесты успешно пройдены!', 'error': False }, status=200)
    if request.user.is_authenticated:
        tr = curtask.results.filter(user=request.user).first()
        if not tr:
            tr = StandaloneTaskResult(taskname=str(curtask._id), success=False,
                points=0, attempts=1, inner_attempts=0, started=now, 
                finished=curtask.until, user=request.user, username=request.user.username)
            tr.save()
            curtask.results.add(tr)
            curtask.save()
    else:
        tr = curtask.results.filter(username=str(request.META.get("REMOTE_ADDR"))).first()
        if not tr:
            tr = StandaloneTaskResult(taskname=str(curtask._id), success=False,
                points=0, attempts=1, inner_attempts=0, started=now, finished=curtask.until,
                username=request.META.get("REMOTE_ADDR"))
            tr.save()
            curtask.results.add(tr)
            curtask.save()
    print(tr)
    #message = 'Внимание! Аргументы находятся в текстовом файле args.txt'
    return render(request, 'OlympiadMainApp/stask.html', {
        'task': curtask, 'title': 'Отправка кода', 'form': form, 'oid': task_oid
        #'cerror': comperr, 'cout': compout, 'out': out, 'err': err, 'score': score, 'rating': rating_map, 'message': message 
        })

@login_required(login_url='login')
@csrf_exempt
def olympiad(request, olymp_oid):
    olymp = Olympiad.objects.get(_id=ObjectId(olymp_oid))

    is_creator = False
    cond = True if olymp.creator != None else False
    if cond and olymp.creator == request.user:
        is_creator = True
        #return redirect('update', task_name=curtask.name)
    
    tasks = olymp.tasks.all()
    results = olymp.results.order_by('-points')

    rating = list(olymp.results.all())
    rating_map = [(i+1,r) for i,r in enumerate(results)]

    return render(request, 'OlympiadMainApp/olympiad.html', {
        'olymp': olymp, 'title': 'Олимпиада', 'rating': rating_map, 'is_creator': is_creator, 'tasks': tasks })

@login_required(login_url='login')
@group_required('Creators')
def update_task(request, task_oid):
    curtask = StandaloneTask.objects.get(_id=ObjectId(task_oid))
    if request.method == 'POST':
        form = SimpleCodeTestDataForm(request.POST)
        if form.is_valid():
            formcontent = form.cleaned_data
            input, output =  formcontent['input'], formcontent['output']
            td = TestData(testinput=input, testoutput=output)
            if isinstance(curtask.tests, list):
                curtask.tests.append(td)
            else:
                curtask.tests = [td]
            curtask.save()
        else:
            print('invalid testdata form')
    else:
        form = SimpleCodeTestDataForm()
    
    return render(request, 'OlympiadMainApp/updatetask.html', {'task': curtask, 'form': form, 'oid': task_oid })

@login_required(login_url='login')
@group_required('Creators')
def edit_stask(request, task_oid):
    task = StandaloneTask.objects.get(_id=ObjectId(task_oid))
    form = EditStaskForm(request.POST or None, instance=task)
    testdataform = SimpleCodeTestDataForm(request.POST or None)
    if form.is_valid():
        print(form.cleaned_data)
    if testdataform.is_valid():
        print(testdataform.cleaned_data)
    #return JsonResponse({'taskname': task.name}, status=200)
    return render(request, 'OlympiadMainApp/editstask.html', {'form': form, 'oid': task_oid, 'testdataform': testdataform })

@login_required(login_url='login')
@group_required('Creators')
def edit_olympiad(request, olymp_oid):
    olympiad = Olympiad.objects.get(_id=ObjectId(olymp_oid))
    form = EditOlympiadForm(request.POST or None, instance=olympiad)
    #testdataform = SimpleCodeTestDataForm(request.POST or None)
    if form.is_valid():
        print(form.cleaned_data)
        form.save()
        messages.success(request, 'Олимпиада успешно модифицирована')
    #return JsonResponse({'taskname': task.name}, status=200)
    return render(request, 'OlympiadMainApp/editolympiad.html', {'form': form, 'oid': olymp_oid })

@login_required(login_url='login')
@group_required('Creators')
def add_itask(request, olymp_oid):
    olympiad = Olympiad.objects.get(_id=ObjectId(olymp_oid))
    form = AddTaskForm(request.POST or None)
    if form.is_valid():
        data = form.cleaned_data
        task = InnerTask.objects.create(
            name=data.get('name'), 
            description=data.get('description'),
            question=data.get('question'),
            points=data.get('points'),
            tasktype=data.get('tasktype'),
            olympiad_id=ObjectId(olymp_oid)
        )
        task = InnerTask.objects.filter(name=task.name, olympiad_id=ObjectId(olymp_oid)).first()
        olympiad.tasks.add(task)
        olympiad.save()
        return JsonResponse({'text': 'task added'}, status=200)
    return render(request, 'OlympiadMainApp/addtask.html', {'form': form, 'oid': olymp_oid})

@login_required(login_url='login')
@group_required('Creators')
def edit_itask(request, task_oid):
    task = InnerTask.objects.get(_id=ObjectId(task_oid))
    olymp_id = task.olympiad_id
    form = AddTaskForm(request.POST or None)
    if task.tasktype == TaskType.CODESIMPLETEST:
        testform = SimpleCodeTestDataForm()
    elif task.tasktype == TaskType.SIMPLECHOICE:
        testform = ChoiceTestDataForm()
    else:
        testform = CodeForm()
    return render(request, 'OlympiadMainApp/edititask.html', {'form': form, 'testform': testform})
    

def view_stask(request, task_oid):
    task = StandaloneTask.objects.get(_id=ObjectId(task_oid))
    #rating = list(task.results.order_by('-points'))
    results = [StandaloneTaskResultViewModel(res) for res in task.results.filter(points__gt=0)]
    results.sort(key=lambda x: x.time)
    #rating_map = [(i+1, StandaloneTaskResultViewModel(r)) for i,r in enumerate(task.results.filter(points__gt=0))]
    rating_map = [(i+1, r) for i,r in enumerate(results)]
    #rating_map.sort(key=lambda tup: tup[1].time)
    return render(request, 'OlympiadMainApp/viewstask.html', {'task': task, 'oid': task_oid, 'rating': rating_map})

@login_required
def view_olympiad(request, olymp_oid):
    task = Olympiad.objects.get(_id=ObjectId(olymp_oid))
    #rating = list(task.results.order_by('-points'))
    results = [StandaloneTaskResultViewModel(res) for res in task.results.filter(points__gt=0)]
    results.sort(key=lambda x: x.time)
    #rating_map = [(i+1, StandaloneTaskResultViewModel(r)) for i,r in enumerate(task.results.filter(points__gt=0))]
    rating_map = [(i+1, r) for i,r in enumerate(results)]
    #rating_map.sort(key=lambda tup: tup[1].time)
    return render(request, 'OlympiadMainApp/viewstask.html', {'task': task, 'oid': olymp_oid, 'rating': rating_map})

@login_required(login_url='login')
@group_required('Creators')
def create_stask(request):
    form = CreateTaskForm(request.POST or None)
    if form.is_valid():
        task = stask_from_form_data(form, request.user)
        task.save()
        print(task)
        return redirect('editstask', task_oid=str(task._id))
    return render(request, 'OlympiadMainApp/createtask.html', {'title': 'Создание задачи', 'form': form })

@login_required(login_url='login')
@group_required('Creators')
def create_olympiad(request):
    form = CreateOlympiadForm(request.POST or None)
    if form.is_valid():
        print(form.cleaned_data)
        data=form.cleaned_data
        olympiad = Olympiad(
            name=data.get('name'), description=data.get('description'),
            since=data.get('since'), until=data.get('until'),
            hidden = data.get('hidden'), active=data.get('active'),
            creator=request.user
        )
        print(data.get('timeout'))
        if data.get('timeout'):
            olympiad.timeout = dt.timedelta(minutes=data.get('timeout'))
        now = datetime.now().astimezone(olympiad.since.tzinfo)
        if olympiad.since <= now:
            olympiad.active = True
        olympiad.created = now
        olympiad.save()
        #return JsonResponse({'text': 'successfully created olympiad'}, status=200)
        return redirect('editolympiad', olymp_oid=str(olympiad._id))
    
    return render(request, 'OlympiadMainApp/createolympiad.html', {'title': 'Создание олимпиады', 'form': form })

def stask_from_form_data(form, user):
    d = form.cleaned_data
    task = StandaloneTask()
    task.name, task.description, task.short_description = d['name'], d['description'], d['short_description']
    task.points = config.STANDALONE_POINTS
    task.since, task.until, task.tasktype = d['since'], d['until'], d['tasktype']
    now = datetime.now().astimezone(task.since.tzinfo)
    if task.since <= now:
        task.active = True
    else:
        task.active = False
    task.creator = user
    task.created = now
    return task

def test(request):
    value = request.session.get("test", 0)
    if value <= 0:
        request.session['test'] = 1
    request.session['test'] = value+1
    return JsonResponse({'times': request.session['test']}, status = 200)

def filetest(request):
    if request.method == "POST":
        print(request.POST)
        print(request.FILES)
        file = request.FILES['file']
        path = default_storage.save('heart_of_the_swarm.txt', ContentFile(file.read()))
        print(path)
    form = FileTestForm(request.POST or None)
    #print(form.file)
    return render(request, 'OlympiadMainApp/testfile.html', {'form': form})
