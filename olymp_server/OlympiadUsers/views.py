from django.shortcuts import render, redirect
from django.urls import reverse_lazy
from django.views import generic
from django.contrib.auth import logout, login, authenticate
from django.contrib.auth.models import Group
from django.contrib import messages
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth.decorators import login_required
from django.contrib import messages

from .forms import *
from .models import Profile

def logout_request(request):
	logout(request)
	messages.info(request, "Вы разлогинились") 
	return redirect("home")

def signin(request):
	
	messages.info(request, "Вы разлогинились") 
	return redirect("home")

def access(request):
    if request.user.is_authenticated:
        print('redirect')
        return redirect("update_profile")
    print(request.POST)
    signup_form = SignUpForm(request.POST or None)
    signin_form = SignInForm(request.POST or None)
    if signin_form.is_valid():
        print(2)
        cd = signin_form.cleaned_data
        if cd['uname'] is not None and cd['pwd'] is not None:
            user = authenticate(username=cd['uname'], password=cd['pwd'])
            if user is not None:
                login(request, user)
                messages.success(request, f'Вы успешно вошли как {user.username} !')
                return redirect('update_profile')
            else:
                messages.error(request, 'Неверные имя пользователя или пароль')
    elif signup_form.is_valid():
        print(1)
        user = signup_form.e
        messages.success(request, f'Вы успешно зарегистрировались как {user.username} !')
        new_user = authenticate(
            username=signup_form.cleaned_data['username'],
            password=signup_form.cleaned_data['password1'],
        )
        login(request, new_user)
        print('redirect')
        return redirect('update_profile')
    return render(request, 'accounts/access.html', {'signup_form': signup_form, 'signin_form': signin_form})

'''
class SignUpView(generic.CreateView):
    form_class = SignUpForm
    success_url = reverse_lazy('login')
    template_name = 'accounts/signup.html'
'''

def signup(request):
    if request.user.is_authenticated:
        print('redirect')
        return redirect("update_profile")
    if request.method == 'POST':
        form = SignUpForm(request.POST)
        user = form.save()
        messages.success(request, f'Вы успешно зарегистрировались как {user.username} !')
        new_user = authenticate(
            username=form.cleaned_data['username'],
            password=form.cleaned_data['password1'],
        )
        login(request, new_user)
        print('redirect')
        return redirect('update_profile')
    else:
        form = SignUpForm()
    return render(request, 'accounts/signup.html', {'title': 'Регистрация', 'form': form})

def ProfileView(request):
    us = request.user
    print('user:', us)
    prof, _ = Profile.objects.get_or_create(user=us)
    if request.method == 'POST':
        form = ProfileForm(request.POST, instance=prof)
        form.save()
    else:
        form = ProfileForm()
    return render(request, 'accounts/profile.html', {'title': 'Профиль', 'form': form})

@login_required
#@transaction.atomic
def update_profile(request):
    user_type_set = request.user.profile.user_type != ''
    if request.method == 'POST':
        user_form = UserForm(request.POST, instance=request.user)
        profile_form = ProfileForm(request.POST, instance=request.user.profile)
        user_type_form = UserTypeForm(request.POST)
        if user_form.is_valid() and profile_form.is_valid() and user_type_form.is_valid():
            user_form.save()
            profile_form.save()
            user_type_data = user_type_form.cleaned_data
            request.user.profile.user_type = user_type_data['usertype']
            if user_type_data['usertype'] == UserType.CREATOR.name:
                group = Group.objects.get(name='Creators')
            else:
                group = Group.objects.get(name='Participants')
            if request.user.groups.filter(name=group.name).count() == 0:
                request.user.groups.add(group)
                request.user.save()
                request.user.save()
            messages.success(request, 'Профиль успешно обновлён!')
            #messages.success(request, _('Профиль успешно обновлён!'))
            #return redirect('settings:profile')
            return render(request, 'accounts/profile.html', {
                'user_form': user_form,
                'profile_form': profile_form,
                'user_type_form': user_type_form,
                'user_type_set': user_type_set,
            })
    else:
        user_form = UserForm(instance=request.user)
        profile_form = ProfileForm(instance=request.user.profile)
        user_type_form = UserTypeForm()
    return render(request, 'accounts/profile.html', {
        'user_form': user_form,
        'profile_form': profile_form,
        'user_type_form': user_type_form,
        'user_type_set': user_type_set,
    })

def view403(request):
    return render(request, 'accounts/403.html')
