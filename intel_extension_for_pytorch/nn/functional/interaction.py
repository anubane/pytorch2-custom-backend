import torch
from torch.autograd import Function

def interaction(*args):
    """
    Get the interaction feature beyond different kinds of features(like
    gender or hobbies), used in DLRM model.
    For now, we only optimized for "dot" interaction
    https://github.com/facebookresearch/dlrm/blob/main/dlrm_s_pytorch.py#L475-L495
    which means we use the dot product to represent the interaction
    feature between two features.
    For example. If feature 1 is "Man" which represented by [0.1, 0.2, 0.3],
    and feature 2 is "Like play football" which represented by [-0.1, 0.3, 0.2].
    The dot interaction feature will be
    ([0.1, 0.2, 0.3] * [-0.1, 0.3, 0.2]^T) =  -0.1 + 0.6 + 0.6 = 1.1
    Args:
        *args: :Multiple tensors which represent different features
    Shape:
        - Input: :math:`N X (B, D)` which N is the number of different kinds of features,
                                          B is the batch size,
                                          D is feature size
        - Output: :math:`(B, D + N*(N-1)/2)`
    """
    if torch.is_grad_enabled():
        return InteractionFunc.apply(*args)
    return torch.ops.torch_ipex.interaction_forward(args)

class InteractionFunc(Function):
    @staticmethod
    def forward(ctx, *args):
        ctx.save_for_backward(*args)
        output = torch.ops.torch_ipex.interaction_forward(args)
        return output

    @staticmethod
    def backward(ctx, grad_out):
        args = ctx.saved_tensors
        grad_in = torch.ops.torch_ipex.interaction_backward(grad_out.contiguous(), args)
        return tuple(grad_in)

